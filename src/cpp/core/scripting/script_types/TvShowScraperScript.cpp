#include <plog/Log.h>

#include "TvShowScraperScript.h"

namespace Scripting::ScriptTypes {
    TvShowScraperScript::TvShowScraperScript(fs::path script_path) {
        _isLoaded = initScript(script_path.string().c_str());
    }
    TvShowScraperScript::TvShowScraperScript(std::string script_path) {
        _isLoaded = initScript(script_path.c_str());
    }

    bool TvShowScraperScript::basicEpisodeSearch(std::string query, std::string scraper_vars, std::string* out) {
        sol::protected_function_result result = basicEpisodeSearchFunc(query, scraper_vars);

        if (!result.valid()) {

            sol::error err = result;
            std::string what = err.what();

            PLOGE.printf("Error in script '%s' function basic_episode_search: '%s'", _path.c_str(), what.c_str());
            return false;
        }
        (*out) = result;
        return true;
    }

    bool TvShowScraperScript::basicSeasonSearch(std::string query, std::string scraper_vars, std::string* out) {
        sol::protected_function_result result = basicSeasonSearchFunc(query, scraper_vars);

        if (!result.valid()) {

            sol::error err = result;
            std::string what = err.what();

            PLOGE.printf("Error in script '%s' function basic_season_search: '%s'", _path.c_str(), what.c_str());
            return false;
        }
        (*out) = result;
        return true;
    }

    bool TvShowScraperScript::basicShowSearch(std::string query, std::string* out) {
        sol::protected_function_result result = basicShowSearchFunc(query);

        if (!result.valid()) {

            sol::error err = result;
            std::string what = err.what();

            PLOGE.printf("Error in script '%s' function basic_show_search: '%s'", _path.c_str(), what.c_str());
            return false;
        }
        (*out) = result;
        return true;
    }

    bool TvShowScraperScript::validate() {
        if (!setAndValidateFunction("basic_episode_search", &basicEpisodeSearchFunc)) { return false; }
        if (!setAndValidateFunction("basic_season_search", &basicSeasonSearchFunc)) { return false; }
        if (!setAndValidateFunction("basic_show_search", &basicShowSearchFunc)) { return false; }
        return true;
    }
}