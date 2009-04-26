#include "lua_script.h"

LuaScript::~LuaScript() {
  if (lua_) lua_close(lua_);
}

const Value LuaScript::call_lua(const char *function_name, const Value &val) {
  int status;
  
  reload_script();
  
  if (!script_ok_) return Value(BAD_REQUEST_ERROR, "Script is broken.");
  
  lua_pushnumber(lua_, worker_->current_time_);
  lua_setglobal(lua_, "current_time");
  
  lua_getglobal(lua_, function_name); /* function to be called */
  if (!val.is_nil()) {
    if (!lua_pushvalue(val)) {
      // TODO: proper error reporting
      std::cout << name_ << "(error): ";
      std::cout << "cannot call '" << function_name << "' with argument " << val << " (type not yet suported in Lua).\n";
      return gNilValue;
    } 
  }
  
  /* Run the function. */
  status = lua_pcall(lua_, (val ? 1 : 0), (retSig ? 1 : 0), 0); // 1 arg, 1 result, no error function
  if (status) {
    // TODO: proper error reporting
    std::cout << name_ << "(error): ";
    std::cout << lua_tostring(lua_, -1) << std::endl;
  	
    script_ok_ = false;
    return gNilValue;
  }
  if (retSig) value_from_lua(retSig);
}

bool LuaScript::eval_script(const std::string &script) {
  int status;
  script_ = script;
  if (!lua_) {
    /* Our own lua context. We might decide to share the context between lua objects some day. */
    lua_ = lua_open();
    /* Load Lua libraries */
    luaL_openlibs(lua_);
    /* push 'this' into the global field 'rubyk_this' */
    lua_pushlightuserdata(lua_, (void*)this);
    lua_setglobal(lua_, "rubyk_this");
    
    /* register methods */
    register_lua_methods();
    
    /* register matrix */
    register_lua_Matrix();
    
    /* open std libs */
    open_lua_libs();
  }
  
  /* set 'current_time' */
  lua_pushnumber(lua_, worker_->current_time_);
  lua_setglobal(lua_, "current_time");
  
  /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
  status = luaL_loadbuffer(lua_, pScript.c_str(), pScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
  if (status) {
    // TODO: proper error reporting
    std::cout << name_ << ": Compilation error !\n";
    std::cout << lua_tostring(lua_, -1) << std::endl;
    return false;
  }
  
  /* Run the script to create the functions. */
  status = lua_pcall(lua_, 0, 0, 0); // 0 arg, 1 result, no error function
  if (status) {
    // TODO: proper error reporting
    std::cout << name_ << ": Function creation failed !\n";
    std::cout << lua_tostring(lua_, -1) << std::endl;
  	return false;
  }
  return true; // ok, we can receive and process values (again).
}

Node * LuaScript::get_node_from_lua(lua_State * L) {
  lua_getglobal(L, "rubyk_this");
  Node * node = (Node*)lua_touserdata(L,lua_gettop(L));
  
  if (!node) printf("Lua error: 'rubyk_this' not set.\n");
  lua_pop(L,1);
  return node;
}

bool LuaScript::real_from_lua(Real * d) {
  int index = lua_gettop(lua_);
  if (!lua_isnumber(lua_, index)) {
    // TODO: proper error reporting
    std::cout << name_ << ": wrong value type to get number (" << lua_typename(lua_, index) << " at " << index << ").\n";
    return false;
  }
  *d = lua_tonumber(lua_,index);
  lua_pop(lua_,1);
  return true;
}

bool LuaScript::string_from_lua(std::string * pStr)
{
  int index = lua_gettop(lua_);
  if (!lua_isstring(lua_, index)) {
    // TODO: proper error reporting
    std::cout << name_ << ": wrong value type to get string (" << lua_typename(lua_, index) << " at " << index << ").\n";
    return false;
  }
  *pStr = lua_tostring(lua_,index);
  lua_pop(lua_,1);
  return true;
}


bool LuaScript::list_from_lua_table(int index, Value *val) { 
  int i  = 1;
  Value tmp;
  if (!lua_istable(lua_, index)) {
    // TODO: proper error reporting
    std::cout << name_ << ": wrong value type to get list (" << lua_typename(lua_, index) << " at " << index << ").\n";
    return false;
  }
  while(true) {
    lua_pushinteger(lua_, i);
    lua_gettable(lua_, index);
    
    if (!value_from_lua(-1, &tmp)) {
      lua_pop(lua_,1);
      break;
    }
    val->push_back(tmp);
    lua_pop(lua_,1);
    i++;
  }    
  lua_pop(lua_,1);
  return true;
}

bool LuaScript::midi_message_from_lua_table(MidiMessage * pMsg, int index)
{ 
  std::string type;
  if (!lua_istable(lua_, index)) {
    // TODO: proper error reporting
    std::cout << name_ << ": wrong value type to get midi message (" << lua_typename(lua_, index) << " at " << index << ").\n";
    return false;
  }
  // type
  lua_getfield(lua_, index, "type");
  if (!lua_isstring(lua_, -1)) {
    // TODO: proper error reporting
    std::cout << name_ << ": could not get type of midi message (" << lua_typename(lua_, -1) << " at " << index << ").\n";
    lua_pop(lua_,1);
    return false;
  }
  type = lua_tostring(lua_, -1);
  lua_pop(lua_,1);
  
  if (type == "NoteOff" || type == "NoteOn") {
    pMsg->set_type(type == "NoteOff" ? NoteOff : NoteOn);
    
    // get note
    lua_getfield(lua_, index, "note");
    if (lua_type(lua_, -1) == LUA_TNUMBER)
      pMsg->set_note(lua_tonumber(lua_, -1));
    else
      pMsg->set_note(MIDI_NOTE_C0);
    lua_pop(lua_,1);
    
    // get channel
    lua_getfield(lua_, index, "channel");
    if (lua_type(lua_, -1) == LUA_TNUMBER)
      pMsg->set_channel(lua_tonumber(lua_, -1));
    else
      pMsg->set_channel(1);
    lua_pop(lua_,1);
    
    // get velocity
    lua_getfield(lua_, index, "velocity");
    if (lua_type(lua_, -1) == LUA_TNUMBER)
      pMsg->set_velocity(lua_tonumber(lua_, -1));
    else
      pMsg->set_velocity(70);
    lua_pop(lua_,1);
    
    // get length
    lua_getfield(lua_, index, "length");
    if (lua_type(lua_, -1) == LUA_TNUMBER)
      pMsg->set_length(lua_tonumber(lua_, -1));
    else
      pMsg->set_length(500);
    lua_pop(lua_,1);
    
  } else if (type == "CtrlChange") {
    // TODO: proper error reporting
    std::cout << name_ << ": CtrlChange from lua not implemented yet...\n";
    return false;
    //lua_pushstring(lua_, "ctrl");
    //lua_pushnumber(lua_, pMessage.ctrl());
    //lua_settable(lua_, -3);
    //lua_pushstring(lua_, "channel");
    //lua_pushnumber(lua_, pMessage.channel());
    //lua_settable(lua_, -3);
    //lua_pushstring(lua_, "value");
    //lua_pushnumber(lua_, pMessage.value());
    //lua_settable(lua_, -3);
  } else {
    // FIXME: raw midi...
  }
  lua_pop(lua_,1);
  return true;
}


bool LuaScript::value_from_lua(int index, Value *val) {
  if (index < 0) index = lua_gettop(lua_) + index + 1;
  
  /* LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
  */
  switch ( lua_type(lua_, index) ) {
  case LUA_TNONE:
    val->set_nil();
    break;
  case LUA_TNIL:
    val->set_nil();
    lua_pop(lua_, 1);
    break;
  case LUA_TNUMBER:
    val->set(lua_tonumber(lua_, index));
    lua_pop(lua_, 1);
    break;
  case LUA_TBOOLEAN:
    val->set((Real)lua_toboolean(lua_, index));
    lua_pop(lua_, 1);
    break;
  case LUA_TTABLE:
    // list or midi message ?
    lua_pushstring(lua_, "type");
    lua_gettable(lua_, index);
    if (lua_isstring(lua_, -1)) {
      // midi
      lua_pop(lua_,1);
      midi_message_from_lua_table(&pMsg, index);
      val->set(&pMsg);
    } else {
      // table
      lua_pop(lua_,1);
      list_from_lua_table(index, val);
    }
    break;
  default:
    // TODO: proper error reporting
    std::cout << name_ << ": wrong value type to build value (" << lua_typename(lua_, lua_type(lua_, index)) << " at " << index << ").\n";
    lua_pop(lua_, 1);
    return false;
  }
  return true;
}


int LuaScript::send(lua_State * L) {
  LuaScript *node = TYPE_CAST(LuaScript, get_node_from_lua(L));
  if (node) {
    // port, value
    Value val;
    Real p;
    if (!node->value_from_lua(&val)) {
      node->error("could not get value");
      return 0;
    }
    if (!node->real_from_lua(&p)) {
      node->error("could not get port from lua in 'send'");
      return 0;
    }
    // check signature
    Outlet *outlet = node->outlet_for_value(p, val);
    if (outlet) {
      outlet->send(val);
    } else {
      // TODO: proper error reporting
      std::cout << name_ << " Could not send " << val << " out (no outlet or wrong value type)\n";
    }
  } else {
    // TODO: proper error reporting
    std::cout << name_ << " Send error: no node\n");
  }
  return 0;
}

int LuaScript::send_note_for_lua(lua_State * L)
{
  LuaScript * node = (LuaScript *)get_node_from_lua(L);
  if (node) {
    Real d;
    size_t port;
    int note, velocity, length, channel;
    time_t when;
    if (!node->real_from_lua(&d)) {
      node->error("could not get time from lua in 'send_note'");
      return 0;
    }
    when = (time_t)d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get channel from lua in 'send_note'");
      return 0;
    }
    channel = d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get length from lua in 'send_note'");
      return 0;
    }
    length = d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get velocity from lua in 'send_note'");
      return 0;
    }
    velocity = d;
    if (lua_istable(node->lua_, lua_gettop(node->lua_))) {
      if (!node->matrix_from_lua_table(&(node->lua_Matrix))) {  
        node->error("could not get note from lua table in 'send_note'");
        return 0;
      } else {
        note = -1;
      }
    } else {
      if (!node->real_from_lua(&d)) {
        node->error("could not get note from lua in 'send_note'");
        return 0;
      } else {
        note = d;
      }
    }
    if (!node->real_from_lua(&d)) {
      node->error("could not get port from lua in 'send_note'");
      return 0;
    }
    port = d;
    if (note == -1) {
      // chord (lua table as notes)
      for (size_t i = 0; i < node->lua_Matrix.size(); i++)
        node->send_note(port, node->lua_Matrix.data[i], velocity, length, channel, when);
    } else
      node->send_note(port, note, velocity, length, channel, when);
  } else {
    printf("send_note_for_lua error: no node\n");
  }
  return 0;
}

int LuaScript::send_ctrl_for_lua(lua_State * L)
{
  LuaScript * node = (LuaScript *)get_node_from_lua(L);
  if (node) {
    Real d;
    size_t port;
    int ctrl, value, channel;
    time_t when;
    if (!node->real_from_lua(&d)) {
      node->error("could not get time from lua in 'send_ctrl'");
      return 0;
    }
    when = d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get channel from lua in 'send_ctrl'");
      return 0;
    }
    channel = d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get value from lua in 'send_ctrl'");
      return 0;
    }
    value = d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get ctrl from lua in 'send_ctrl'");
      return 0;
    }
    ctrl = d;
    if (!node->real_from_lua(&d)) {
      node->error("could not get port from lua in 'send_ctrl'");
      return 0;
    }
    port = d;
    node->send_ctrl(port, ctrl, value, channel, when);
  } else {
    printf("send_ctrl_for_lua error: no node\n");
  }
  return 0;
}


bool LuaScript::matrix_from_lua (lua_State *L, Matrix ** pMat, int index)
{
  Matrix ** mat_ptr, * mat;
  mat_ptr = (Matrix **)luaL_checkudata(L, index, LUA_MATRIX_NAME);
  if (mat_ptr == NULL) {
    std::cout << "lua: wrong value type to get matrix (" << lua_typename(L, lua_type(L, index)) << " at " << index << ").\n";
    return false;
  } else {
    mat = *mat_ptr;
    if (!mat) {
      std::cout << "lua: null matrix from lua.\n";
      return false;
    }
    *pMat = mat;
    return true;
  }
}

void LuaScript::set_lua_global (const char * key, const Value &val)
{
  if (lua_pushvalue(val)) {
    lua_setglobal(lua_, key);
  } else if (val.type) {
    // TODO: proper error reporting
    std::cout << name_ << "(error): ";
    // TODO: proper error reporting
    std::cout << "cannot set '" << key << "' to " << val << " (type not yet suported in Lua).\n";
  }
}


bool LuaScript::lua_has_function (const char * key)
{
  bool res;
  lua_getglobal(lua_, key);
  res = lua_isfunction(lua_, -1);
  lua_pop(lua_,-1);
  return res;
}

bool LuaScript::lua_pushvalue(const Value &val) {
  switch (val.type()) {
    case REAL_VALUE:
      lua_pushnumber(lua_, val.r);
      break;
    case STRING_VALUE:
      lua_pushstring(lua_, val.str().c_str());
      break;
    case LIST_VALUE:
      lua_pushlist(val);
      break;
    case ERROR_VALUE:
      // TODO
      break;
    case HASH_VALUE:
      // TODO
      break;
    case MATRIX_VALUE:
      // TODO
      break;
    case MIDI_VALUE:
      lua_pushmidi(val.midi_message_);
      break;
    case ANY_VALUE:
      // TODO ??
      break;
    case EMPTY_VALUE:
      /* we consider that if you set a value with empty it means you want Nil. 
       * This is very useful for return values. */
      /* continue */
    case NIL_VALUE:
      /* continue */
    default:
      lua_pushnil(lua_);
  }
  
  return true;
}

void LuaScript::lua_pushmidi (const MidiMessage& pMessage)
{
  // fixme create a new 'midi' table class...
  // lua_newtable(lua_);
  // lua_pushstring(lua_, pMessage.type_name());
  // lua_setfield(lua_, -2, "type");
  // 
  // if (pMessage.mType == NoteOff || pMessage.mType == NoteOn) {
  //   lua_pushnumber(lua_, pMessage.note());
  //   lua_setfield(lua_, -2, "note");
  //   
  //   lua_pushnumber(lua_, pMessage.channel());
  //   lua_setfield(lua_, -2, "channel");
  //   
  //   lua_pushnumber(lua_, pMessage.velocity());
  //   lua_setfield(lua_, -2, "velocity");
  //   
  //   lua_pushnumber(lua_, pMessage.length());
  //   lua_setfield(lua_, -2, "length");
  //   
  // } else if (pMessage.mType == CtrlChange) {
  //   lua_pushnumber(lua_, pMessage.ctrl());
  //   lua_setfield(lua_, -2, "ctrl");
  //   
  //   lua_pushnumber(lua_, pMessage.channel());
  //   lua_setfield(lua_, -2, "channel");
  //   
  //   lua_pushnumber(lua_, pMessage.value());
  //   lua_setfield(lua_, -2, "value");
  // } else {
  //   // FIXME: raw midi...
  // }
}

void LuaScript::lua_pushlist(const Value &val) {
  lua_newtable(lua_);
  
  for (size_t i = 0; i < val.size(); ++i) {
    // TODO...
  }
}

int LuaScript::Matrix_get (lua_State *L)
{
  Matrix * mat;
  if (!matrix_from_lua(L, &mat, 1)) return 0;
  int index = luaL_checkint(L, 2) - 1;
  if (index < 0 || index >= (int)mat->size()) return 0;
  lua_pushnumber(L, mat->data[index]);
  return 1;
}

int LuaScript::Matrix_gc (lua_State *L)
{
  // nothing to do for gc (const ref).
  return 0;
}

int LuaScript::Matrix_tostring (lua_State *L)
{ 
  Matrix * mat;
  if (!matrix_from_lua(L, &mat, 1)) return 0;
  
  std::ostringstream oss(std::ostringstream::out);
  oss << *mat;
  lua_pushfstring(L, "%s", oss.str().c_str());
  return 1;
}


const luaL_reg LuaScript::sMatrix_methods[] = {
  {"get",        Matrix_get},
  {"tostring",   Matrix_tostring},
  {0,0}
};

const luaL_reg LuaScript::sMatrix_meta[] = {
  {"__gc",       Matrix_gc},
  {"__tostring", Matrix_tostring},
  {0, 0}
};

void LuaScript::register_lua_methods()
{
  /* register methods for lua */
  Hash_iterator it,end;
  const THash<std::string, method_for_lua_t> * method_for_lua = class_->methodsForLua();
  end   = method_for_lua->end();
  
  /* register send methods */
  lua_pushcfunction(lua_, &LuaScript::send);
  lua_setglobal(lua_, "send");
  
  lua_pushcfunction(lua_, &LuaScript::send_note);
  lua_setglobal(lua_, "send_note");
  
  lua_pushcfunction(lua_, &LuaScript::send_ctrl);
  lua_setglobal(lua_, "send_ctrl");
  
  lua_pushcfunction(lua_, &LuaScript::send_ctrl);
  lua_setglobal(lua_, "send_ctrl");
  
  for(it = method_for_lua->begin(); it < end; it++) {
    method_for_lua_t method;
    if (method_for_lua->get(&method, *it)) {
      lua_pushcfunction(lua_, method);
      lua_setglobal(lua_, (*it).c_str());
    }
  }
}

void LuaScript::register_lua_Matrix()
{
  luaL_newmetatable(lua_, LUA_MATRIX_NAME);        /* create metatable for Matrix, add it to the Lua registry */
  luaL_openlib(lua_, LUA_MATRIX_NAME, sMatrix_methods, 0);  /* create methods table, add it to the globals */
  
  lua_pushstring(lua_, "__index");
  lua_pushstring(lua_, "get");
  lua_gettable(lua_, 2);  /* get Matrix.get */
  lua_settable(lua_, 1);  /* metatable.__index = Matrix.get */
  
  
  lua_pushstring(lua_, "__tostring");
  lua_pushstring(lua_, "tostring");
  lua_gettable(lua_, 2); /* get Matrix.tostring */
  lua_settable(lua_, 1); /* metatable.__tostring = Matrix.tostring */
  
  
  // uncomment when we have const / not const checking code.
  // lua_pushstring(L, "__newindex");
  // lua_pushstring(L, "set");
  // lua_gettable(L, 2); /* get array.set */
  // lua_settable(L, 1); /* metatable.__newindex = array.set */
  
  //  luaL_openlib(lua_, 0, sMatrix_meta, 0);  /* fill metatable */
  //  lua_pushliteral(lua_, "__index");
  //  lua_pushvalue(lua_, -3);               /* dup methods table*/
  //  lua_rawset(lua_, -3);                  /* metatable.__index = methods */
  //  lua_pushliteral(lua_, "__metatable");
  //  lua_pushvalue(lua_, -3);               /* dup methods table*/
  //  lua_rawset(lua_, -3);                  /* hide metatable:
  //                                         metatable.__metatable = methods */
  //  lua_pop(lua_, 1);                      /* drop metatable */
}

void LuaScript::open_lua_lib(const char* pName, lua_CFunction pFunc)  
{  
  lua_pushcfunction(lua_, pFunc) ;  
  lua_pushstring(lua_, pName) ;  
  lua_call(lua_, 1, 0) ;  
}

void LuaScript::open_lua_libs()
{
  open_base_lua_libs();
}

void LuaScript::open_base_lua_libs()
{
  open_lua_lib("", luaopen_base);
  open_lua_lib(LUA_TABLIBNAME, luaopen_table);
  open_lua_lib(LUA_IOLIBNAME, luaopen_io);
  open_lua_lib(LUA_STRLIBNAME, luaopen_string);
  open_lua_lib(LUA_MATHLIBNAME, luaopen_math);
}