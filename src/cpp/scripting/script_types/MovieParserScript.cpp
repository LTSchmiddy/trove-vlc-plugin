#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "MovieParserScript.h"
#include "scripting/lua_utils.h"


namespace Scripting::ScriptTypes {
    // Public:
    MovieParserScript::MovieParserScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    MovieParserScript::MovieParserScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }

    // Protected:
    bool MovieParserScript::validate() {
        if (!setAndValidateFunction("parse_path", &parsePathFunc)) { return false; }
        return true;
    }
}