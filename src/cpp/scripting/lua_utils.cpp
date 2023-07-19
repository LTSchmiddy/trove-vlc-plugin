#include "lua_utils.h"

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
    }
}