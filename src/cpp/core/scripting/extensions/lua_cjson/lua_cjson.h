#pragma once

#include <lua.h>

#ifdef __cplusplus
extern "C" {
#endif

int luaopen_cjson(lua_State *l);
int luaopen_cjson_safe(lua_State *l);

#ifdef __cplusplus
}
#endif