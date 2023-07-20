#pragma once

#include <sol/sol.hpp>
#include <string>

#include "ns_abbr/fs.h"

namespace Scripting {

    class MovieScraperScript {
    public:
        sol::state lua = NULL;

        MovieScraperScript(fs::path script_path);
        MovieScraperScript(std::string script_path);
        ~MovieScraperScript();

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