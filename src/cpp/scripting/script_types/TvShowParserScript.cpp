#include <plog/Log.h>

#include "TvShowParserScript.h"

namespace Scripting::ScriptTypes {
    TvShowParserScript::TvShowParserScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    TvShowParserScript::TvShowParserScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }

    std::string TvShowParserScript::parsePath(std::string directory, std::string filename) {
        sol::protected_function_result result = parsePathFunc(directory, filename);

        if (!result.valid()) {
            sol::error err = result;
            std::string what = err.what();

            PLOGE.printf("Error in script '%s' function parse_path: '%s'", _path.c_str(), what.c_str());
            return "{}";
        }

        return (std::string)result;
    }

    bool TvShowParserScript::validate() {
        if (!setAndValidateFunction("parse_path", &parsePathFunc)) { return false; }
        return true;
    }
}