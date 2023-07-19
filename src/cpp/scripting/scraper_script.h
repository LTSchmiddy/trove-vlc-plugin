#pragma once

#include <sol/sol.hpp>
#include <string>

#include "fs.h"

namespace Scripting {

    class ScraperScript {
    public:
        sol::state lua = NULL;

        ScraperScript(fs::path script_path);
        ScraperScript(std::string script_path);
        ~ScraperScript();

        bool isLoaded();
        std::string getPath();

        bool basicSearch(std::string query, std::string* out);

    private:
        std::string _path;
        bool _isLoaded = false;
        sol::protected_function searchFunc;

        bool initScript(const char* script_path);
        bool setAndValidateFunction(std::string func_name, sol::protected_function* func);

    };
}