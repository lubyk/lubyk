extern "C" {
// we compiled Lua as C code
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "lua_script.h"
#include "lua_inlet.h"

#define RUBYK_THIS_IN_LUA "__this"

const Value LuaScript::lua_init() {
  // Our own lua context.
  lua_ = lua_open();
  
  // Load Lua main libraries
  luaL_openlibs(lua_);
  
  // push 'this' into the global field '__this'
  lua_pushlightuserdata(lua_, (void*)this);
  lua_setglobal(lua_, "__this");
  
  register_lua_method<LuaScript, &LuaScript::lua_inlet>("inlet");
  
  // load rubyk.lua
  Value res = root_->call(LIB_URL);
  if (!res.is_string()) {
    return res;
  } else {
    std::string path(res.str());
    path.append("/lua/rubyk.lua");
    int status = luaL_dofile(lua_, path.c_str());
    if (status) {
      return Value(INTERNAL_SERVER_ERROR, 
        std::string(lua_tostring(lua_, -1)).append("."));
    }
  }
  return gNilValue;
}

LuaScript::~LuaScript() {
  lua_close(lua_);
}

const Value LuaScript::call_lua(const char *function_name, const Value &val) {
  int status;

  reload_script();

  if (!is_ok()) return Value(BAD_REQUEST_ERROR, "Script is broken.");

  lua_pushnumber(lua_, worker_->current_time_);
  lua_setglobal(lua_, "current_time");

  lua_getglobal(lua_, function_name); /* function to be called */
  if (!lua_pushvalue(val)) {
    return Value(BAD_REQUEST_ERROR, std::string("cannot call '").append(function_name).append("' with argument ").append(val.lazy_json()).append(" (type not yet suported in Lua).\n"));
  }
  
  /* Run the function. */
  status = lua_pcall(lua_, 1, 1, 0); // 1 arg, 1 result, no error function
  if (status) {
    return Value(BAD_REQUEST_ERROR, lua_tostring(lua_, -1));
  }
  
  return stack_to_value();
}


const Value LuaScript::eval_script() {
  int status;
  
  /* set 'current_time' */
  lua_pushnumber(lua_, worker_->current_time_);
  lua_setglobal(lua_, "current_time");
  
  // compile script
  status = luaL_loadbuffer(lua_, script_.c_str(), script_.size(), name_.c_str());
  if (status) {
    return Value(BAD_REQUEST_ERROR, std::string(lua_tostring(lua_, -1)).append("."));
  }
  
  // Run the script to create the functions.
  status = lua_pcall(lua_, 0, 0, 0); // 0 arg, 1 result, no error function
  if (status) {
    // TODO: proper error reporting
      return Value(BAD_REQUEST_ERROR, std::string(lua_tostring(lua_, -1)).append("."));
  }
  // ok, we can receive and process values (again).
  return Value(script_);
}

void LuaScript::register_lua_method(const char *name, lua_CFunction function) {
  lua_pushcfunction(lua_, function);
  lua_setglobal(lua_, name);
}

LuaScript *LuaScript::lua_this(lua_State *L) {
  lua_getglobal(L, RUBYK_THIS_IN_LUA);
  LuaScript *script = (LuaScript*)lua_touserdata(L,lua_gettop(L));
  if (!script) fprintf(stderr, "Lua error: '__this' not set.\n");
  lua_pop(L,1);
  return script;
}

int LuaScript::lua_inlet(const Value &val) {
  if (!val.is_list() || !val[0].is_string() || val[1].size() < 2 ) {
    // invalid call to 'inlet'
    // TODO: proper error reporting
    return 0;
  }
  Object *in = child("in");
  if (in) {
    Object *inlet = in->child(val[0].str().c_str());
    if (inlet != NULL && (inlet->name() != val[0].str() || inlet->type_id() != val[1][0].type_id())) {
      // TODO: inlet rename = keep connections
      // TODO: inlet type change = reset connections
    } else if (inlet == NULL) {
      in->adopt(new LuaInlet(this, val[0].str().c_str(), val[1]));
    } else {
      // TODO: update units/range/...
    }
  } else {
    fprintf(stderr, "%s: Could not find inlet folder 'in'\n", name_.c_str());
  }
  return 0;
}

const Value LuaScript::stack_to_value() {
  int top = lua_gettop(lua_);
  Value res, tmp;
  
  for (int i = 1; i <= top; ++i) {
    if (value_from_lua(i, &tmp)) {
      res.push_back(tmp);
    } else {
      break;
    }
  }
  lua_pop(lua_, top);
  return res;
}

bool LuaScript::value_from_lua(int index, Value *res) {
  int top = lua_gettop(lua_);
  
  if (index < 0) {
    index = top + index + 1; // -1 == top
  }
  
  if (index > top) {
    // nothing
    return false;
  }
  
  
  /* LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
  */
  switch ( lua_type(lua_, index) ) {
  case LUA_TNONE:
    return false;
  case LUA_TNIL:
    res->set_nil();
    break;
  case LUA_TNUMBER:
    res->set(lua_tonumber(lua_, index));
    break;
  case LUA_TSTRING:
    res->set(lua_tostring(lua_, index));
    break;
  case LUA_TBOOLEAN:
    res->set((Real)lua_toboolean(lua_, index));
    break;
  case LUA_TTABLE:
    // list or midi message ?
    lua_pushstring(lua_, "type");
    lua_gettable(lua_, index);
    if (lua_isstring(lua_, -1)) {
      lua_pop(lua_,1); // type
      // midi
      // midi_message_from_lua_table(&pMsg, index);
      // res->set(&pMsg);
      res->set_nil();
    } else {  
      lua_pop(lua_,1); // type
      // table
      res->set_empty();
      return list_from_lua(index, res);
    }
    break;
  default:
    // TODO: proper error reporting
    std::cerr << name_ << ": wrong value type to build value (" << lua_typename(lua_, lua_type(lua_, index)) << " at " << index << ").\n";
    return false;
  }
  return true;
}


bool LuaScript::list_from_lua(int index, Value *val) {
  int size = lua_objlen(lua_, index);
  Value tmp;
  
  for(int i = 1; i <= size; ++i) {
    
    lua_rawgeti(lua_, index, i); // no meta table passing
    
    if (!value_from_lua(-1, &tmp)) {
      // TODO: unsupported format. BAD
      val->push_back(gNilValue);
    } else {
      val->push_back(tmp);
    }
    lua_pop(lua_,1);
  }
  
  std::cout << "list_from_lua: " << *val << "\n";
  return true;
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
      /* continue */
    case HASH_VALUE:
      // TODO
      /* continue */
    case MATRIX_VALUE:
      // TODO
      /* continue */
    case MIDI_VALUE:
      // lua_pushmidi(val.midi_message_);
      // TODO
      /* continue */
    case ANY_VALUE:
      // TODO ??
      /* continue */
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


bool LuaScript::lua_pushlist(const Value &val) {
  lua_createtable(lua_, val.size(), 0); // top
  
  for (size_t i = 0; i < val.size(); ++i) {
    lua_pushnumber(lua_, i + 1);
    if (!lua_pushvalue(val[i])) {
      return false;
    }
    lua_settable(lua_, -3); // stack = ..., {}, i, VALUE
  }
  return true;
}

void LuaScript::dump_stack(const char *msg, int index) {
  int i;
  int top = lua_gettop(lua_);

  printf("%s @ %i (%d)\n", msg, index, top);

  for (i = 1; i <= top; ++i) {
    switch (lua_type(lua_, i)) {
      case LUA_TSTRING:
        printf("%3i: '%s'\n", i, lua_tostring(lua_, i));
        break;
      case LUA_TBOOLEAN:
        printf("%3i: %s\n", i, lua_toboolean(lua_, i) ? "true" : "false");
        break;
      case LUA_TNUMBER:
        printf("%3i: %g\n", i, lua_tonumber(lua_, i));
        break;
      case LUA_TTABLE:
        printf("%3i: {...}\n", i);
        break;
      case LUA_TNONE:
        printf("%3i: --\n", i);
      case LUA_TNIL:
        printf("%3i: nil\n", i);
        break;
      default:
        printf("%3i: %s\n", i, lua_typename(lua_, i));
    }
  }
  printf("\n");  /* end the listing */
}