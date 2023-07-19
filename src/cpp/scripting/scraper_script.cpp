#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "scraper_script.h"
#include "extensions/lua_log.h"
#include "extensions/lua_web_requests.h"
#include "extensions/lua_json.h"


namespace Scripting {
    // Constructors:
    ScraperScript::ScraperScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    ScraperScript::ScraperScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }
    ScraperScript::~ScraperScript() {}

    // Public:
    bool ScraperScript::isLoaded() {
        return _isLoaded;
    }
    std::string ScraperScript::getPath() {
        return _path;
    }

    bool ScraperScript::basicSearch(std::string query, std::string* out) {
        // sol::protected_function_result result = searchFunc(query);

        // if (!result.valid()) {
        //     PLOGE.printf("Error in script '%s' search function", _path.c_str());
        //     return false;
        // }

        // (*out) = result;
        (*out) = searchFunc(query);

        return true;
    }

    // Private:
    bool ScraperScript::initScript(const char* script_path) {
        _path = script_path;
        lua.open_libraries(
            sol::lib::base
        );

        Scripting::Extensions::Logging::create_lua_log_module(&lua);
        Scripting::Extensions::Json::create_lua_json_module(&lua, true);
        Scripting::Extensions::WebRequests::create_lua_web_requests_module(&lua);

        sol::protected_function_result result;

        try {
            result = lua.safe_script_file(script_path);
            if (!result.valid()) {
                sol::error err = result;
                std::string what = err.what();

                PLOGE.printf("Failed to run script '%s': %s", script_path, err.what());
                return false;
            }
        } catch (std::exception& e) {
            PLOGE.printf("Script '%s' has errors.", script_path);
            return false;
        }

        // Checking that script has all of it's necessary components:
        searchFunc = lua["search"];
        if (searchFunc.valid()) {
            PLOGD.printf("Search function found in script '%s", script_path);
        } else {
            PLOGE.printf("Script '%s' has no search function", script_path);
            return false;
        }

        return true;
    }

}