#pragma once
#include <string>
#include <sol/sol.hpp>

namespace Scripting::Extensions::Logging {

    void create_lua_log_module(sol::state* lua);
}