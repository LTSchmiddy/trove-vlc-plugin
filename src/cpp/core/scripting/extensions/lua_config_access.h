#pragma once

#include <sol/sol.hpp>
#include <string>

namespace Scripting::Extensions::ConfigAccess {

void create_lua_config_access_module(sol::state_view* lua);
}