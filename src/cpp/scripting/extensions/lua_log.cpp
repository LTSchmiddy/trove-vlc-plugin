#include "lua_log.h"


#include <plog/Log.h>

namespace Scripting::Extensions::Logging {
    
    static void _lua_log_verbose(std::string val) {
        PLOGV << val;
    }
    static void _lua_log_debug(std::string val) {
        PLOGD << val;
    }
    static void _lua_log_info(std::string val) {
        PLOGI << val;
    }
    static void _lua_log_warning(std::string val) {
        PLOGW << val;
    }
    static void _lua_log_error(std::string val) {
        PLOGE << val;
    }
    static void _lua_log_fatal(std::string val) {
        PLOGF << val;
    }

    void create_lua_log_module(sol::state* lua) {
        sol::table log_table = lua->create_named_table("log");

        log_table.set_function("verbose", &_lua_log_verbose);
        log_table.set_function("debug", &_lua_log_debug);
        log_table.set_function("info", &_lua_log_info);
        log_table.set_function("warning", &_lua_log_warning);
        log_table.set_function("error", &_lua_log_error);
        log_table.set_function("fatal", &_lua_log_fatal);
    }
}