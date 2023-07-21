#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "MovieScraperScript.h"
#include "scripting/lua_utils.h"


namespace Scripting::ScriptTypes {
    MovieScraperScript::MovieScraperScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    MovieScraperScript::MovieScraperScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
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

    // Protected:
    bool MovieScraperScript::validate() {
        if (!setAndValidateFunction("search", &searchFunc)) { return false; }

        return true;
    }
}