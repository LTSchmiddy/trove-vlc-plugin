#pragma once

#include <string>
#include <sol/sol.hpp>

namespace Scripting::Extensions::WebRequests {
    
    void _lua_post_request(std::string address, sol::table args);

    void create_lua_web_requests_module (sol::state* lua);
}