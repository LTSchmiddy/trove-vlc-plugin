#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "MovieParserScript.h"
#include "core/scripting/lua_utils.h"


namespace Scripting::ScriptTypes {
    
    MovieParserScript::MovieParserScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    MovieParserScript::MovieParserScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }

    std::string MovieParserScript::parsePath(std::string directory, std::string filename) {
        sol::protected_function_result result = parsePathFunc(directory, filename);

        if (!result.valid()) {
            sol::error err = result;
            std::string what = err.what();

            PLOGE.printf("Error in script '%s' function parse_path: '%s'", _path.c_str(), what.c_str());
            return "{}";
        }

        return (std::string)result;
    }

    // Protected:
    bool MovieParserScript::validate() {
        if (!setAndValidateFunction("parse_path", &parsePathFunc)) { return false; }
        return true;
    }
}