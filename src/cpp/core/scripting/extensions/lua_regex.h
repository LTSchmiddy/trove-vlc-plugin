#pragma once

#include <sol/sol.hpp>

namespace Scripting::Extensions::Regex {
    void create_lua_regex_module (sol::state_view* lua);
}