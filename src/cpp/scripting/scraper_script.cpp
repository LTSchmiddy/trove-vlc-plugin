#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "scraper_script.h"


namespace Scripting {



    ScraperScript::ScraperScript(fs::path script_path) {
        initScript(script_path.string().c_str());
    }
    ScraperScript::ScraperScript(std::string script_path) {
        initScript(script_path.c_str());
    }
    ScraperScript::~ScraperScript() {

    }


    bool ScraperScript::initScript(const char* script_path) {
        state.open_libraries(
            sol::lib::base
        );

        sol::protected_function_result result;

        try {
            result = state.safe_script_file(script_path);
            if (!result.valid()) {
                sol::error err = result;
                std::string what = err.what();

                PLOGE.printf("Failed to run script '%s': %s", script_path, err.what());
                return false;
            }
        } catch (std::exception& e) {
            PLOGE.printf("Script '%s' has syntax errors.", script_path);
            return false;
        }

        // Checking that script has all of it's necessary components:
        searchFn = state["search"];
        if (searchFn.valid()) {
            PLOGD.printf("Search function found in script '%s", script_path);
        } else {
            PLOGE.printf("Script '%s' has no search function", script_path);
            return false;
        }

        return true;
    }

}