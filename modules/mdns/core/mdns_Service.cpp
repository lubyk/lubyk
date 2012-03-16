/**
 *
 * MACHINE GENERATED FILE. DO NOT EDIT.
 *
 * Bindings for class Service
 *
 * This file has been generated by dub 2.1.~.
 */
#include "dub/dub.h"
#include "mdns/Service.h"

using namespace mdns;

/** mdns::Service::Service(std::string service_type, const char *name, int interface_index, const char *type, const char *domain, bool is_add)
 * include/mdns/Service.h:71
 */
static int Service_Service(lua_State *L) {
  try {
    size_t service_type_sz_;
    const char *service_type = dub_checklstring(L, 1, &service_type_sz_);
    const char *name = dub_checkstring(L, 2);
    int interface_index = dub_checkint(L, 3);
    const char *type = dub_checkstring(L, 4);
    const char *domain = dub_checkstring(L, 5);
    bool is_add = dub_checkboolean(L, 6);
    Service *retval__ = new Service(std::string(service_type, service_type_sz_), name, interface_index, type, domain, is_add);
    retval__->pushobject(L, retval__, "mdns.Service", true);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Service: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Service: Unknown exception");
  }
  return dub_error(L);
}

/** virtual mdns::Service::~Service()
 * include/mdns/Service.h:73
 */
static int Service__Service(lua_State *L) {
  try {
    DubUserdata *userdata = ((DubUserdata*)dub_checksdata_d(L, 1, "mdns.Service"));
    if (userdata->gc) {
      Service *self = (Service *)userdata->ptr;
      delete self;
    }
    userdata->gc = false;
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "~Service: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "~Service: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize mdns::Service::fd(lua_State *L)
 * include/mdns/Service.h:75
 */
static int Service_fd(lua_State *L) {
  try {
    Service *self = *((Service **)dub_checksdata(L, 1, "mdns.Service"));
    return self->fd(L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fd: Unknown exception");
  }
  return dub_error(L);
}

/** bool mdns::Service::isAdd()
 * include/mdns/Service.h:84
 */
static int Service_isAdd(lua_State *L) {
  try {
    Service *self = *((Service **)dub_checksdata(L, 1, "mdns.Service"));
    lua_pushboolean(L, self->isAdd());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isAdd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isAdd: Unknown exception");
  }
  return dub_error(L);
}

/** const char* mdns::Service::name()
 * include/mdns/Service.h:88
 */
static int Service_name(lua_State *L) {
  try {
    Service *self = *((Service **)dub_checksdata(L, 1, "mdns.Service"));
    lua_pushstring(L, self->name());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize mdns::Service::serviceType(lua_State *L)
 * include/mdns/Service.h:92
 */
static int Service_serviceType(lua_State *L) {
  try {
    Service *self = *((Service **)dub_checksdata(L, 1, "mdns.Service"));
    return self->serviceType(L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "serviceType: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "serviceType: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize mdns::Service::info(lua_State *L)
 * include/mdns/Service.h:99
 */
static int Service_info(lua_State *L) {
  try {
    Service *self = *((Service **)dub_checksdata(L, 1, "mdns.Service"));
    return self->info(L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "info: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "info: Unknown exception");
  }
  return dub_error(L);
}



// --=============================================== __tostring
static int Service___tostring(lua_State *L) {
  Service *self = *((Service **)dub_checksdata_n(L, 1, "mdns.Service"));
  lua_pushfstring(L, "mdns.Service: %p (%s)", self, self-> name());
  
  return 1;
}

// --=============================================== METHODS

static const struct luaL_Reg Service_member_methods[] = {
  { "new"          , Service_Service      },
  { "__gc"         , Service__Service     },
  { "fd"           , Service_fd           },
  { "isAdd"        , Service_isAdd        },
  { "name"         , Service_name         },
  { "serviceType"  , Service_serviceType  },
  { "info"         , Service_info         },
  { "__tostring"   , Service___tostring   },
  { NULL, NULL},
};


extern "C" int luaopen_mdns_Service(lua_State *L)
{
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mdns.Service");
  // <mt>

  // register member methods
  luaL_register(L, NULL, Service_member_methods);
  // save meta-table in mdns
  dub_register(L, "mdns", "Service_core");
  // <mt>
  lua_pop(L, 1);
  return 0;
}
