#pragma once

#include <string>
#include <sol/sol.hpp>


namespace Scripting::Extensions::WebRequests {
    
    void create_lua_web_requests_module (sol::state_view* lua);
}