#pragma once

#include <string>
#include <sol/sol.hpp>


namespace Scripting::Extensions::WebRequests {
    
    // std::string _lua_post_request(std::string address, std::string args, std::list<std::string> header);
    // std::string _lua_get_request(std::string address, std::list<std::string> header);

    void create_lua_web_requests_module (sol::state* lua);
}