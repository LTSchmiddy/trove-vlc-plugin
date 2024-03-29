#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "MovieScraperScript.h"
#include "core/scripting/lua_utils.h"


namespace Scripting::ScriptTypes {
    MovieScraperScript::MovieScraperScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    MovieScraperScript::MovieScraperScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }

    
    bool MovieScraperScript::basicSearch(std::string query, std::string* out) {
        sol::protected_function_result result = basicSearchFunc(query);

        if (!result.valid()) {

            sol::error err = result;
            std::string what = err.what();

            PLOGE.printf("Error in script '%s' function basic_search: '%s'", _path.c_str(), what.c_str());
            return false;
        }
        (*out) = result;
        return true;
    }

    // Protected:
    bool MovieScraperScript::validate() {
        if (!setAndValidateFunction("basic_search", &basicSearchFunc)) { return false; }

        return true;
    }
}