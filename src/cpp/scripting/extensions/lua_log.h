#pragma once
#include <string>
#include <sol/sol.hpp>

namespace Scripting::Extensions::Logging {
    
    void _lua_log_verbose(std::string val);
    void _lua_log_debug(std::string val);
    void _lua_log_info(std::string val);
    void _lua_log_warning(std::string val);
    void _lua_log_error(std::string val);
    void _lua_log_fatal(std::string val);

    void create_lua_log_module(sol::state* lua);
}