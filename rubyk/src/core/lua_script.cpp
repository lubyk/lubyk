#include "lua_script.h"

LuaScript::~LuaScript()
{
  if (mLua) lua_close(mLua);
}


bool LuaScript::set_lua (const Value& p)
{
  return set_script(p);
}

void LuaScript::call_lua (Value * retSig, const char * pFunctionName, const Value * sig)
{
  int status;
  
  reload_script();
  
  if (!mScriptOK) return;
  
  lua_pushnumber(mLua, mRoot->mCurrentTime);
  lua_setglobal(mLua, "current_time");
  
  lua_getglobal(mLua, pFunctionName); /* function to be called */
  if (sig) {
    if (!lua_pushsignal(*sig)) {
      *mOutput << mName << "(error): ";
      *mOutput << "cannot call '" << pFunctionName << "' with argument " << *sig << " (type not yet suported in Lua).\n";
      return;
    } 
  }
  
  /* Run the function. */
  status = lua_pcall(mLua, (sig ? 1 : 0), (retSig ? 1 : 0), 0); // 1 arg, 1 result, no error function
  if (status) {
    *mOutput << mName << "(error): ";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
  	
    mScriptOK = false;
    return;
  }
  if (retSig) signal_from_lua(retSig);
}

bool LuaScript::eval_script(const std::string& pScript) 
{
  mScript = pScript;
  
  return eval_lua_script(mScript);
}

bool LuaScript::eval_lua_script(const std::string& pScript)
{
  int status;
  if (!mLua) {
    /* Our own lua context. We might decide to share the context between lua objects some day. */
    mLua = lua_open();
    /* Load Lua libraries */
    luaL_openlibs(mLua);
    /* push 'this' into the global field 'rubyk_this' */
    lua_pushlightuserdata(mLua, (void*)this);
    lua_setglobal(mLua, "rubyk_this");
    
    /* register methods */
    register_lua_methods();
    
    /* register matrix */
    register_lua_Matrix();
    
    /* open std libs */
    open_lua_libs();
  }
  
  /* set 'current_time' */
  lua_pushnumber(mLua, mRoot->mCurrentTime);
  lua_setglobal(mLua, "current_time");
  
  /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
  status = luaL_loadbuffer(mLua, pScript.c_str(), pScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
  if (status) {
    *mOutput << mName << ": Compilation error !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
    return false;
  }
  
  /* Run the script to create the functions. */
  status = lua_pcall(mLua, 0, 0, 0); // 0 arg, 1 result, no error function
  if (status) {
    *mOutput << mName << ": Function creation failed !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
  	return false;
  }
  return true; // ok, we can receive and process signals (again).
}

Node * LuaScript::get_node_from_lua(lua_State * L)
{
  lua_getglobal(L, "rubyk_this");
  Node * node = (Node*)lua_touserdata(L,lua_gettop(L));
  
  if (!node) printf("Lua error: 'rubyk_this' not set.\n");
  lua_pop(L,1);
  return node;
}

bool LuaScript::signal_from_lua(Value * sig, int index)
{
  return signal_from_lua(sig, index, mLuaMatrix, mLuaMidiMessage);
}

bool LuaScript::real_from_lua(real_t * d)
{
  int index = lua_gettop(mLua);
  if (!lua_isnumber(mLua, index)) {
    *mOutput << mName << ": wrong value type to get number (" << lua_typename(mLua, index) << " at " << index << ").\n";
    return false;
  }
  *d = lua_tonumber(mLua,index);
  lua_pop(mLua,1);
  return true;
}

bool LuaScript::string_from_lua(std::string * pStr)
{
  int index = lua_gettop(mLua);
  if (!lua_isstring(mLua, index)) {
    *mOutput << mName << ": wrong value type to get string (" << lua_typename(mLua, index) << " at " << index << ").\n";
    return false;
  }
  *pStr = lua_tostring(mLua,index);
  lua_pop(mLua,1);
  return true;
}

bool LuaScript::matrix_from_lua_table(Matrix * pMat)
{
  return matrix_from_lua_table(pMat, lua_gettop(mLua));
}

bool LuaScript::matrix_from_lua_table(Matrix * pMat, int pIndex)
{ 
  int i  = 1;
  if (!pMat->set_sizes(1,0)) return false;
  if (!lua_istable(mLua, pIndex)) {
    *mOutput << mName << ": wrong value type to get matrix (" << lua_typename(mLua, pIndex) << " at " << pIndex << ").\n";
    return false;
  }
  while(true) {
    lua_pushinteger(mLua, i);
    lua_gettable(mLua, pIndex);
    if (!lua_isnumber(mLua, -1)) {
      lua_pop(mLua,1);
      break;
    }
    real_t d = lua_tonumber(mLua, -1);
    pMat->append(d);
    lua_pop(mLua,1);
    i++;
  }    
  lua_pop(mLua,1);
  return true;
}

bool LuaScript::midi_message_from_lua_table(MidiMessage * pMsg)
{
  return midi_message_from_lua_table(pMsg, lua_gettop(mLua));
}

bool LuaScript::midi_message_from_lua_table(MidiMessage * pMsg, int pIndex)
{ 
  std::string type;
  if (!lua_istable(mLua, pIndex)) {
    *mOutput << mName << ": wrong value type to get midi message (" << lua_typename(mLua, pIndex) << " at " << pIndex << ").\n";
    return false;
  }
  // type
  lua_getfield(mLua, pIndex, "type");
  if (!lua_isstring(mLua, -1)) {
    *mOutput << mName << ": could not get type of midi message (" << lua_typename(mLua, -1) << " at " << pIndex << ").\n";
    lua_pop(mLua,1);
    return false;
  }
  type = lua_tostring(mLua, -1);
  lua_pop(mLua,1);
  
  if (type == "NoteOff" || type == "NoteOn") {
    pMsg->set_type(type == "NoteOff" ? NoteOff : NoteOn);
    
    // get note
    lua_getfield(mLua, pIndex, "note");
    if (lua_type(mLua, -1) == LUA_TNUMBER)
      pMsg->set_note(lua_tonumber(mLua, -1));
    else
      pMsg->set_note(MIDI_NOTE_C0);
    lua_pop(mLua,1);
    
    // get channel
    lua_getfield(mLua, pIndex, "channel");
    if (lua_type(mLua, -1) == LUA_TNUMBER)
      pMsg->set_channel(lua_tonumber(mLua, -1));
    else
      pMsg->set_channel(1);
    lua_pop(mLua,1);
    
    // get velocity
    lua_getfield(mLua, pIndex, "velocity");
    if (lua_type(mLua, -1) == LUA_TNUMBER)
      pMsg->set_velocity(lua_tonumber(mLua, -1));
    else
      pMsg->set_velocity(70);
    lua_pop(mLua,1);
    
    // get length
    lua_getfield(mLua, pIndex, "length");
    if (lua_type(mLua, -1) == LUA_TNUMBER)
      pMsg->set_length(lua_tonumber(mLua, -1));
    else
      pMsg->set_length(500);
    lua_pop(mLua,1);
    
  } else if (type == "CtrlChange") {
    *mOutput << mName << ": CtrlChange from lua not implemented yet...\n";
    return false;
    //lua_pushstring(mLua, "ctrl");
    //lua_pushnumber(mLua, pMessage.ctrl());
    //lua_settable(mLua, -3);
    //lua_pushstring(mLua, "channel");
    //lua_pushnumber(mLua, pMessage.channel());
    //lua_settable(mLua, -3);
    //lua_pushstring(mLua, "value");
    //lua_pushnumber(mLua, pMessage.value());
    //lua_settable(mLua, -3);
  } else {
    // FIXME: raw midi...
  }
  lua_pop(mLua,1);
  return true;
}


bool LuaScript::signal_from_lua(Value * sig, int pIndex, Matrix& pMat, MidiMessage& pMsg)
{
  int index = pIndex < 0 ? lua_gettop(mLua) + pIndex + 1 : pIndex;
  /* LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
  */
  switch ( lua_type(mLua, index) ) {
  case LUA_TNONE:
    sig->set_bang();
    break;
  case LUA_TNIL:
    sig->set_nil();
    lua_pop(mLua, 1);
    break;
  case LUA_TNUMBER:
    sig->set(lua_tonumber(mLua, index));
    lua_pop(mLua, 1);
    break;
  case LUA_TBOOLEAN:
    sig->set(lua_toboolean(mLua, index));
    lua_pop(mLua, 1);
    break;
  case LUA_TTABLE:
    // matrix or midi message ?
    lua_pushstring(mLua, "type");
    lua_gettable(mLua, index);
    if (lua_isstring(mLua, -1)) {
      lua_pop(mLua,1);
      midi_message_from_lua_table(&pMsg, index);
      sig->set(&pMsg);
    } else {
      lua_pop(mLua,1);
      matrix_from_lua_table(&pMat, index);
      sig->set(pMat);
    }
    break;
  default:
    *mOutput << mName << ": wrong value type to build signal (" << lua_typename(mLua, lua_type(mLua, index)) << " at " << index << ").\n";
    lua_pop(mLua, 1);
    return false;
  }
  return true;
}


int LuaScript::send_for_lua(lua_State * L)
{
  LuaScript * node = (LuaScript *)get_node_from_lua(L);
  if (node) {
    // port, value
    Value sig;
    real_t p;
    if (!node->signal_from_lua(&sig)) {
      node->error("could not get signal");
      return 0;
    }
    if (!node->real_from_lua(&p)) {
      node->error("could not get port from lua in 'send'");
      return 0;
    }
    node->send((int)p, sig);
  } else {
    printf("send_for_lua error: no node\n");
  }
  return 0;
}

int LuaScript::send_note_for_lua(lua_State * L)
{
  LuaScript * node = (LuaScript *)get_node_from_lua(L);
  if (node) {
    real_t d;
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
    if (lua_istable(node->mLua, lua_gettop(node->mLua))) {
      if (!node->matrix_from_lua_table(&(node->mLuaMatrix))) {  
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
      for (size_t i = 0; i < node->mLuaMatrix.size(); i++)
        node->send_note(port, node->mLuaMatrix.data[i], velocity, length, channel, when);
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
    real_t d;
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


bool LuaScript::matrix_from_lua (lua_State *L, Matrix ** pMat, int pIndex)
{
  Matrix ** mat_ptr, * mat;
  mat_ptr = (Matrix **)luaL_checkudata(L, pIndex, LUA_MATRIX_NAME);
  if (mat_ptr == NULL) {
    std::cout << "lua: wrong value type to get matrix (" << lua_typename(L, lua_type(L, pIndex)) << " at " << pIndex << ").\n";
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

void LuaScript::set_lua_global (const char * key, const Value val)
{
  if (lua_pushsignal(sig)) {
    lua_setglobal(mLua, key);
  } else if (val.type) {
    *mOutput << mName << "(error): ";
    *mOutput << "cannot set '" << key << "' to " << sig << " (type not yet suported in Lua).\n";
  }
}


bool LuaScript::lua_has_function (const char * key)
{
  bool res;
  lua_getglobal(mLua, key);
  res = lua_isfunction(mLua, -1);
  lua_pop(mLua,-1);
  return res;
}

bool LuaScript::lua_pushsignal (const Value val)
{
  real_t d;
  const Matrix * live;
  if (val.get(&live)) {
    lua_pushmatrix(*live);
  } else if (val.type == MidiValue) {
    lua_pushmidi(*(val.midi_ptr.value));
  } else if (val.get(&d)) {
    lua_pushnumber(mLua, d);
  } else if (val.is_bang()) {
    lua_pushnil(mLua);
  } else {
    return false;
  }
  return true;
}

void LuaScript::lua_pushmidi (const MidiMessage& pMessage)
{
  // fixme create a new 'midi' table class...
  lua_newtable(mLua);
  lua_pushstring(mLua, pMessage.type_name());
  lua_setfield(mLua, -2, "type");
  
  if (pMessage.mType == NoteOff || pMessage.mType == NoteOn) {
    lua_pushnumber(mLua, pMessage.note());
    lua_setfield(mLua, -2, "note");
    
    lua_pushnumber(mLua, pMessage.channel());
    lua_setfield(mLua, -2, "channel");
    
    lua_pushnumber(mLua, pMessage.velocity());
    lua_setfield(mLua, -2, "velocity");
    
    lua_pushnumber(mLua, pMessage.length());
    lua_setfield(mLua, -2, "length");
    
  } else if (pMessage.mType == CtrlChange) {
    lua_pushnumber(mLua, pMessage.ctrl());
    lua_setfield(mLua, -2, "ctrl");
    
    lua_pushnumber(mLua, pMessage.channel());
    lua_setfield(mLua, -2, "channel");
    
    lua_pushnumber(mLua, pMessage.value());
    lua_setfield(mLua, -2, "value");
  } else {
    // FIXME: raw midi...
  }
}

void LuaScript::lua_pushmatrix (const Matrix& pMat)
{
  Matrix ** mat_ptr = (Matrix **)lua_newuserdata(mLua, sizeof(Matrix *));
  *mat_ptr = const_cast<Matrix*>(&pMat); // we are carefull not to modify the matrix (matrix from)
  luaL_getmetatable(mLua, LUA_MATRIX_NAME);
  lua_setmetatable(mLua, -2);
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
  const THash<std::string, method_for_lua_t> * method_for_lua = mClass->methodsForLua();
  end   = method_for_lua->end();
  
  /* register send methods */
  lua_pushcfunction(mLua, &send_for_lua);
  lua_setglobal(mLua, "send");
  
  lua_pushcfunction(mLua, &send_note_for_lua);
  lua_setglobal(mLua, "send_note");
  
  lua_pushcfunction(mLua, &send_ctrl_for_lua);
  lua_setglobal(mLua, "send_ctrl");
  
  for(it = method_for_lua->begin(); it < end; it++) {
    method_for_lua_t method;
    if (method_for_lua->get(&method, *it)) {
      lua_pushcfunction(mLua, method);
      lua_setglobal(mLua, (*it).c_str());
    }
  }
}

void LuaScript::register_lua_Matrix()
{
  luaL_newmetatable(mLua, LUA_MATRIX_NAME);        /* create metatable for Matrix, add it to the Lua registry */
  luaL_openlib(mLua, LUA_MATRIX_NAME, sMatrix_methods, 0);  /* create methods table, add it to the globals */
  
  lua_pushstring(mLua, "__index");
  lua_pushstring(mLua, "get");
  lua_gettable(mLua, 2);  /* get Matrix.get */
  lua_settable(mLua, 1);  /* metatable.__index = Matrix.get */
  
  
  lua_pushstring(mLua, "__tostring");
  lua_pushstring(mLua, "tostring");
  lua_gettable(mLua, 2); /* get Matrix.tostring */
  lua_settable(mLua, 1); /* metatable.__tostring = Matrix.tostring */
  
  
  // uncomment when we have const / not const checking code.
  // lua_pushstring(L, "__newindex");
  // lua_pushstring(L, "set");
  // lua_gettable(L, 2); /* get array.set */
  // lua_settable(L, 1); /* metatable.__newindex = array.set */
  
  //  luaL_openlib(mLua, 0, sMatrix_meta, 0);  /* fill metatable */
  //  lua_pushliteral(mLua, "__index");
  //  lua_pushvalue(mLua, -3);               /* dup methods table*/
  //  lua_rawset(mLua, -3);                  /* metatable.__index = methods */
  //  lua_pushliteral(mLua, "__metatable");
  //  lua_pushvalue(mLua, -3);               /* dup methods table*/
  //  lua_rawset(mLua, -3);                  /* hide metatable:
  //                                         metatable.__metatable = methods */
  //  lua_pop(mLua, 1);                      /* drop metatable */
}

void LuaScript::open_lua_lib(const char* pName, lua_CFunction pFunc)  
{  
  lua_pushcfunction(mLua, pFunc) ;  
  lua_pushstring(mLua, pName) ;  
  lua_call(mLua, 1, 0) ;  
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