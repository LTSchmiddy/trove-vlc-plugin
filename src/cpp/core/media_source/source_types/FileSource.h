#pragma once

#include "core/library/containers/MovieContainer.h"
#include "core/library/containers/TvEpisodeContainer.h"
#include "core/library/containers/TvSeasonContainer.h"
#include "core/library/containers/TvShowContainer.h"
#include "core/library/containers/TvShowSearchTextContainer.h"
#include "core/ns_abbr/fs.h"
#include "core/ns_abbr/json.h"
#include <string>

#include "SourceBase.h"

namespace MediaSource::SourceType {
class FileSource : public SourceBase {
  public:
    // Override functions:
    std::string getType() override;
    std::string getUriPrefix() override;
    void loadTypeSettings(json& settings_json) override;
    void saveTypeSettings(json& settings_json) override;
    void scanForMovies(std::stop_token stoken,
                       std::string src_name,
                       Scripting::ScriptTypes::MovieParserScript* parser,
                       Scripting::ScriptTypes::MovieScraperScript* scraper) override;
    void scanForTvShows(std::stop_token stoken,
                        std::string src_name,
                        Scripting::ScriptTypes::TvShowParserScript* parser,
                        Scripting::ScriptTypes::TvShowScraperScript* scraper) override;
    SOURCE_GETSET(std::string, _rootPath);

  private:
    std::string _rootPath;
    void scanDirectoryForMovies(std::stop_token stoken,
                                std::string src_name,
                                fs::path dir,
                                std::list<fs::path>* p_selected_files);
    void scanDirectoryForTvShows(std::stop_token stoken,
                                 std::string src_name,
                                 fs::path dir,
                                 Scripting::ScriptTypes::TvShowParserScript* parser,
                                 Scripting::ScriptTypes::TvShowScraperScript* scraper);

    void scrapeTvShow(std::stop_token stoken,
                      json& show_params,
                      json& scraper_vars,
                      Library::Containers::TvShowContainer* show,
                      Scripting::ScriptTypes::TvShowParserScript* parser,
                      Scripting::ScriptTypes::TvShowScraperScript* scraper);

    void scrapeTvSeason(std::stop_token stoken,
                        json& season_params,
                        json& scraper_vars,
                        Library::Containers::TvSeasonContainer* show,
                        Scripting::ScriptTypes::TvShowParserScript* parser,
                        Scripting::ScriptTypes::TvShowScraperScript* scraper);

    void scrapeTvEpisode(std::stop_token stoken,
                         json& season_params,
                         json& scraper_vars,
                         Library::Containers::TvEpisodeContainer* episode,
                         Scripting::ScriptTypes::TvShowParserScript* parser,
                         Scripting::ScriptTypes::TvShowScraperScript* scraper);
};
} // namespace MediaSource::SourceType