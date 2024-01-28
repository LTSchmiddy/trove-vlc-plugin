#include "lua_utils.h"
#include "extensions/lua_config_access.h"
#include "extensions/lua_log.h"
#include "extensions/lua_web_requests.h"
#include "extensions/lua_json.h"
#include "extensions/lua_regex.h"

namespace Scripting::Utils {
    void lua_open_all_libraries(sol::state_view* lua){
        lua->open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::os,
            sol::lib::math,
            sol::lib::table,
            sol::lib::debug,
            sol::lib::bit32,
            sol::lib::io,
            sol::lib::utf8
        );

        Scripting::Extensions::ConfigAccess::create_lua_config_access_module(lua);
        Scripting::Extensions::Logging::create_lua_log_module(lua);
        Scripting::Extensions::Json::create_lua_json_module(lua, true);
        Scripting::Extensions::WebRequests::create_lua_web_requests_module(lua);
        Scripting::Extensions::Regex::create_lua_regex_module(lua);
    }
}