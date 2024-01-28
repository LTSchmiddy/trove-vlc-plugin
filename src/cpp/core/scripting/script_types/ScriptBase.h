#pragma once

#include <sol/sol.hpp>
#include <string>

#include "core/ns_abbr/fs.h"

namespace Scripting::ScriptTypes {

    class ScriptBase {
    public:
        ScriptBase();
        ScriptBase(fs::path script_path);
        ScriptBase(std::string script_path);
        virtual ~ScriptBase();

        bool isLoaded();
        std::string getPath();

    protected:
        sol::state lua;
        std::string _path;
        bool _isLoaded = false;
        bool initScript(const char* script_path);
        bool setAndValidateFunction(std::string func_name, sol::protected_function* func);

        virtual bool validate();
    };
}