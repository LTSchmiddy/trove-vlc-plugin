#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "MovieParserScript.h"
#include "scripting/lua_utils.h"


namespace Scripting::ScriptTypes {
    
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