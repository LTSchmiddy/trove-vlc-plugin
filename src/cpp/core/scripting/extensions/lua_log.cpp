#include <format>
#include <plog/Log.h>
#include "lua_log.h"

namespace Scripting::Extensions::Logging {

    static std::string _lua_log_format_for_log (sol::object val) {
        switch (val.get_type()) {
            case sol::type::none:
                return std::format("*Nothing*");
            case sol::type::lua_nil:
                return std::format("nil");
            case sol::type::string:
                return std::format("\'{}\'", val.as<std::string>());
            case sol::type::number:
                return std::format("{}", val.as<float>());
            case sol::type::boolean:
                return std::format("{}", val.as<bool>());
            case sol::type::function:
                return std::format("Function at address '{}'", val.pointer());
            case sol::type::userdata:
                return std::format("Userdata at address {}", val.pointer());
            case sol::type::lightuserdata:
                return std::format("Lightuserdata at address '{}'", val.pointer());
            case sol::type::table:
                return std::format("Table at address '{}'", val.pointer());
            case sol::type::poly:
                return std::format("Poly at address '{}'", val.pointer());
            default:
                return std::format("Unknown object at address '{}'", val.pointer());
        }

    }
    static void _lua_log_verbose(sol::object val) {
        PLOGV << _lua_log_format_for_log(val);
    }
    static void _lua_log_debug(sol::object val) {
        PLOGD << _lua_log_format_for_log(val);
    }
    static void _lua_log_info(sol::object val) {
        PLOGI << _lua_log_format_for_log(val);
    }
    static void _lua_log_warning(sol::object val) {
        PLOGW << _lua_log_format_for_log(val);
    }
    static void _lua_log_error(sol::object val) {
        PLOGE << _lua_log_format_for_log(val);
    }
    static void _lua_log_fatal(sol::object val) {
        PLOGF << _lua_log_format_for_log(val);
    }

    void create_lua_log_module(sol::state_view* lua) {
        sol::table log_table = lua->create_named_table("log");

        log_table.set_function("verbose", &_lua_log_verbose);
        log_table.set_function("debug", &_lua_log_debug);
        log_table.set_function("info", &_lua_log_info);
        log_table.set_function("warning", &_lua_log_warning);
        log_table.set_function("error", &_lua_log_error);
        log_table.set_function("fatal", &_lua_log_fatal);
    }
}