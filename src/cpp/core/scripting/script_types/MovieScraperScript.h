#pragma once

#include <sol/sol.hpp>
#include <string>

#include "core/ns_abbr/fs.h"
#include "ScriptBase.h"

namespace Scripting::ScriptTypes {

    class MovieScraperScript : public ScriptBase {
    public:
        MovieScraperScript(fs::path script_path);
        MovieScraperScript(std::string script_path);

        bool basicSearch(std::string query, std::string* out);

    protected:
        sol::protected_function basicSearchFunc;

        bool validate() override;

    };
}