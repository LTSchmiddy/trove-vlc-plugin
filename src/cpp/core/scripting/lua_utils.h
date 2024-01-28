#pragma once

#include <sol/sol.hpp>

namespace Scripting::Utils {
    void lua_open_all_libraries(sol::state_view* lua);
}