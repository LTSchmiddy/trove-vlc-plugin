#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "scraper_script.h"
#include "extensions/lua_log.h"
#include "extensions/lua_web_requests.h"
#include "extensions/lua_json.h"
#include "lua_utils.h"


namespace Scripting {
    // Constructors:
    MovieScraperScript::MovieScraperScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    MovieScraperScript::MovieScraperScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }
    MovieScraperScript::~MovieScraperScript() {}

    // Public:
    bool MovieScraperScript::isLoaded() {
        return _isLoaded;
    }
    std::string MovieScraperScript::getPath() {
        return _path;
    }

    bool MovieScraperScript::basicSearch(std::string query, std::string* out) {
        sol::protected_function_result result = searchFunc(query);

        if (!result.valid()) {
            PLOGE.printf("Error in script '%s' search function", _path.c_str());
            return false;
        }

        (*out) = result;

        return true;
    }

    // Private:
    bool MovieScraperScript::initScript(const char* script_path) {
        _path = script_path;
        Utils::lua_open_all_libraries(&lua);

        Scripting::Extensions::Logging::create_lua_log_module(&lua);
        Scripting::Extensions::Json::create_lua_json_module(&lua, true);
        Scripting::Extensions::WebRequests::create_lua_web_requests_module(&lua);

        // Load the script:
        sol::protected_function_result result;
        try {
            result = lua.safe_script_file(_path);
            if (!result.valid()) {
                sol::error err = result;
                std::string what = err.what();

                PLOGE.printf("Failed to run script '%s': %s", _path.c_str(), err.what());
                return false;
            }
        } catch (std::exception& e) {
            PLOGE.printf("Script '%s' has errors.", _path.c_str());
            return false;
        }
        
        // Checking that script has all of it's necessary components:
        if (!setAndValidateFunction("search", &searchFunc)) { return false; }

        return true;
    }

    bool MovieScraperScript::setAndValidateFunction(std::string func_name, sol::protected_function* func) {
        *func = lua[func_name];
        if (func->valid()) {
            PLOGD.printf("Function '%s' found in script '%s", func_name.c_str(), _path.c_str());
            return true;
        }

        PLOGE.printf("Function '%s' not found in script '%s", func_name.c_str(), _path.c_str());
        return false;

    }

}