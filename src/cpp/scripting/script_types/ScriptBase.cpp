#include <fstream>
#include <Plog/Log.h>
#include <functional>

#include "ScriptBase.h"
#include "scripting/lua_utils.h"


namespace Scripting::ScriptTypes {
    // Constructors:
    ScriptBase::ScriptBase() {}

    ScriptBase::ScriptBase(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    ScriptBase::ScriptBase(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }
    ScriptBase::~ScriptBase() {}

    // Public:
    bool ScriptBase::isLoaded() {
        return _isLoaded;
    }
    std::string ScriptBase::getPath() {
        return _path;
    }

    // Private:
    bool ScriptBase::initScript(const char* script_path) {
        _path = script_path;
        Utils::lua_open_all_libraries(&lua);

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
        return validate();
    }

    bool ScriptBase::setAndValidateFunction(std::string func_name, sol::protected_function* func) {
        *func = lua[func_name];
        if (func->valid()) {
            PLOGD.printf("Function '%s' found in script '%s", func_name.c_str(), _path.c_str());
            return true;
        }

        PLOGE.printf("Function '%s' not found in script '%s", func_name.c_str(), _path.c_str());
        return false;
    }

    bool ScriptBase::validate() {return true;}

}