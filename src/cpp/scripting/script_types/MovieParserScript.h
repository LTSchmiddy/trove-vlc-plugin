#pragma once

#include <sol/sol.hpp>
#include <vector>
#include <string>

#include "ns_abbr/fs.h"
#include "ScriptBase.h"

namespace Scripting::ScriptTypes {

    class MovieParserScript : public ScriptBase {
    public:
        using ScriptBase::ScriptBase;

        std::string parsePath(std::string directory, std::string filename);

    protected:
        sol::protected_function parsePathFunc;
        bool validate() override;

    };
}