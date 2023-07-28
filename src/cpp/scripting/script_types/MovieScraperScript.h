#pragma once

#include <sol/sol.hpp>
#include <string>

#include "ns_abbr/fs.h"
#include "ScriptBase.h"

namespace Scripting::ScriptTypes {

    class MovieScraperScript : public ScriptBase {
    public:
        using ScriptBase::ScriptBase;

        bool basicSearch(std::string query, std::string* out);

    protected:
        sol::protected_function basicSearchFunc;

        bool validate() override;

    };
}