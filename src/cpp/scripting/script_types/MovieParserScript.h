#pragma once

#include <sol/sol.hpp>
#include <string>

#include "ns_abbr/fs.h"
#include "ScriptBase.h"

namespace Scripting::ScriptTypes {

    class MovieParserScript : public ScriptBase {
    public:
        MovieParserScript(fs::path script_path);
        MovieParserScript(std::string script_path);

    protected:
        bool validate() override;

    };
}