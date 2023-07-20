#pragma once

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

namespace Scripting::Extensions::Json {
    void create_lua_json_module(sol::state_view* lua, bool safe_mode);
    
}