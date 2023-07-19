#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "scraper_script.h"
#include "extensions/lua_log.h"
#include "extensions/lua_web_requests.h"
#include "extensions/lua_json.h"


namespace Scripting {

    ScraperScript::ScraperScript(fs::path script_path) {
        loadSuccessful = initScript(script_path.string().c_str());
    }
    ScraperScript::ScraperScript(std::string script_path) {
        loadSuccessful = initScript(script_path.c_str());
    }
    ScraperScript::~ScraperScript() {}


    bool ScraperScript::initScript(const char* script_path) {
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
        searchFn = lua["search"];
        if (searchFn.valid()) {
            PLOGD.printf("Search function found in script '%s", script_path);
        } else {
            PLOGE.printf("Script '%s' has no search function", script_path);
            return false;
        }

        return true;
    }

}