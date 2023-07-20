#include "lua_json.h"
#include "lua_cjson/lua_cjson.h"

namespace Scripting::Extensions::Json {
    void create_lua_json_module(sol::state_view* lua, bool safe_mode) {
        if (safe_mode) {
            luaopen_cjson_safe(lua->lua_state());
        }
        else {
            luaopen_cjson(lua->lua_state());
        }

    }
}