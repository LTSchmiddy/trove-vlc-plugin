#pragma once

#include <sol/sol.hpp>
#include <string>

#include "ns_abbr/fs.h"
#include "ScriptBase.h"

namespace Scripting::ScriptTypes {

    class TvShowScraperScript : public ScriptBase {
    public:
        TvShowScraperScript(fs::path script_path);
        TvShowScraperScript(std::string script_path);

        bool basicEpisodeSearch(std::string query, std::string* out);
        bool basicSeasonSearch(std::string query, std::string* out);
        bool basicShowSearch(std::string query, std::string* out);

    protected:
        sol::protected_function basicEpisodeSearchFunc;
        sol::protected_function basicSeasonSearchFunc;
        sol::protected_function basicShowSearchFunc;

        bool validate() override;

    };
}