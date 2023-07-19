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

    private:
        bool loadSuccessful = false;
        sol::protected_function searchFn;

        bool initScript(const char* script_path);

    };
}