#pragma once

#include <sol/sol.hpp>
#include <vector>
#include <string>

#include "ns_abbr/fs.h"
#include "ScriptBase.h"

namespace Scripting::ScriptTypes {

    class TvShowParserScript : public ScriptBase {
    public:
        TvShowParserScript(fs::path script_path);
        TvShowParserScript(std::string script_path);

        std::string parsePath(std::string directory, std::string filename);

    protected:
        sol::protected_function parsePathFunc;
        bool validate() override;

    };
}