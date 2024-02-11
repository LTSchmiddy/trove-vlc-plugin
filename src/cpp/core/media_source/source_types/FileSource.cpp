#include <algorithm>

#include <plog/Log.h>

#include "FileSource.h"
#include "core/settings/settings_globals.h"

namespace MediaSource::SourceType {
std::string FileSource::getType() { return "file"; }
std::string FileSource::getUriPrefix() {
#ifdef _WIN32
    return "file:///" + get_rootPath();
#else
    return "file://" + get_rootPath();
#endif
}

void FileSource::loadTypeSettings(json& settings_json) {
    if (settings_json.contains("rootPath"))
        set_rootPath(settings_json["rootPath"].get<std::string>());
}
void FileSource::saveTypeSettings(json& settings_json) { settings_json["rootPath"] = get_rootPath(); }

void FileSource::scanForMovies(std::stop_token stoken,
                               std::string src_name,
                               Scripting::ScriptTypes::MovieParserScript* parser,
                               Scripting::ScriptTypes::MovieScraperScript* scraper) {
    scanDirectoryForMovies(stoken, src_name, fs::path(get_rootPath()), parser, scraper);
}

void FileSource::scanForTvShows(std::stop_token stoken,
                                std::string src_name,
                                Scripting::ScriptTypes::TvShowParserScript* parser,
                                Scripting::ScriptTypes::TvShowScraperScript* scraper) {
    scanDirectoryForTvShows(stoken, src_name, fs::path(get_rootPath()), parser, scraper);
}

void FileSource::scanDirectoryForMovies(std::stop_token stoken,
                                        std::string src_name,
                                        fs::path dir,
                                        Scripting::ScriptTypes::MovieParserScript* parser,
                                        Scripting::ScriptTypes::MovieScraperScript* scraper) {
    
    // First, we need to find all of the movie files to process.
    std::list<fs::path> selected_files;                                        
    // Iterate source directory:
    for (const fs::directory_entry& entry : fs::directory_iterator(dir)) {
        // We'll need re-check the stop token at multiple points, since some checks can take a while:
        // Here's the first check, on a new iteration of the loop:
        if (stoken.stop_requested()) {
            return;
        }

        if (entry.is_directory()) {
            PLOGV.printf("Scanning Directory %s", entry.path().string().c_str());
            scanDirectoryForMovies(stoken, src_name, entry, parser, scraper);

            // If the stop was called in a recursive call, return from here:
            if (stoken.stop_requested()) {
                return;
            }

        } else if (entry.is_regular_file()) {
            PLOGV.printf("Found Movie File %s", entry.path().string().c_str());

            // Skip this file if it's not a usable extension:
            if (std::find(Global::settings.file_extensions.begin(),
                          Global::settings.file_extensions.end(),
                          entry.path().extension().string()) == Global::settings.file_extensions.end()) {
                continue;
            }

            // We're still operating on relative paths:
            fs::path rel_path = fs::relative(entry.path(), get_rootPath());
            selected_files.push_back(rel_path);
        }
    }

    int progress = 0;
    PLOG_DEBUG.printf("Found: %i files", selected_files.size());
    // Now, we begin scraping for metadata:
    for (std::list<fs::path>::iterator it = selected_files.begin(); it != selected_files.end(); ++it) {
        fs::path rel_path = *it;

        // Updating scraping progress:
        {
            std::lock_guard lock(progressGuard);
            _scanProgress = (float)progress / (float)selected_files.size();
            _currentScanPath = rel_path.string();

            PLOG_DEBUG.printf("Progress: %d", _scanProgress);
        }
        Library::Containers::MovieContainer new_movie = Library::Containers::MovieContainer(src_name, rel_path.string(), false);

        // Check for a stop again before we do any db access:
        if (stoken.stop_requested()) {
            return;
        }

        // This will enable us to skip movies we've already scanned
        if (!new_movie.existsInDb()) {
            std::string search_params = parser->parsePath(rel_path.parent_path().string(), rel_path.filename().string());

            // Check for a stop again after scraping (expecially since scraping requires internet access and can be time consuming):
            if (stoken.stop_requested()) {
                return;
            }
            std::string query_results_str;
            scraper->basicSearch(search_params, &query_results_str);

            PLOGD << query_results_str;
            json query_results_json = json::parse(query_results_str);

            if (query_results_json.contains("title"))
                new_movie.title = query_results_json["title"].get<std::string>();
            if (query_results_json.contains("date"))
                new_movie.date = query_results_json["date"].get<std::string>();
            if (query_results_json.contains("desc"))
                new_movie.desc = query_results_json["desc"].get<std::string>();
            if (query_results_json.contains("poster_path"))
                new_movie.poster_path = query_results_json["poster_path"].get<std::string>();

            new_movie.writeToDb();
            
            // Increase scraping progress:

        }
        progress++;
    }

    // Clearing the scan progress:
    {
        std::lock_guard lock(progressGuard);
        _scanProgress = 0;
        _currentScanPath = "";
    }
}

void FileSource::scanDirectoryForTvShows(std::stop_token stoken,
                                         std::string src_name,
                                         fs::path dir,
                                         Scripting::ScriptTypes::TvShowParserScript* parser,
                                         Scripting::ScriptTypes::TvShowScraperScript* scraper) {
    for (const fs::directory_entry& entry : fs::directory_iterator(dir)) {
        // We'll need re-check the stop token at multiple points, since some checks can take a while:
        // Here's the first check, on a new iteration of the loop:
        if (stoken.stop_requested()) {
            return;
        }

        if (entry.is_directory()) {
            PLOGV.printf("Scanning Directory %s", entry.path().string().c_str());
            scanDirectoryForTvShows(stoken, src_name, entry, parser, scraper);

            // If the stop was called in a recursive call, return from here:
            if (stoken.stop_requested()) {
                return;
            }

        } else if (entry.is_regular_file()) {
            PLOGV.printf("Scanning Tv Show File %s", entry.path().string().c_str());

            // Skip this file if it's not a usable extension:
            if (std::find(Global::settings.file_extensions.begin(),
                          Global::settings.file_extensions.end(),
                          entry.path().extension().string()) == Global::settings.file_extensions.end()) {
                continue;
            }

            fs::path rel_path = fs::relative(entry.path(), get_rootPath());

            // PLOGV << src_name;

            // Loading a database container for the file:
            Library::Containers::TvEpisodeContainer episode(
                src_name, rel_path.string(), false); // We don't need to look up the stored episode data right now.

            // Check for a stop again before we do any db access:
            if (stoken.stop_requested()) {
                return;
            }

            // No need to do anything if the file is already in the db.
            if (!episode.existsInDb()) {
                Library::Containers::TvShowContainer show;
                Library::Containers::TvShowSearchTextContainer show_search_text;

                // Parse file path for search info:
                std::string search_params_str = parser->parsePath(rel_path.parent_path().string(), rel_path.filename().string());
                // Check for a stop again after scraping (expecially since scraping requires internet access and can be time consuming):
                json search_params = json::parse(search_params_str);
                // Verifying result:
                if (!search_params.contains("show")) {
                    PLOGE.printf("Parser script '%s' did not return table 'show'", parser->getPath().c_str());
                    return;
                }
                if (!search_params.contains("season")) {
                    PLOGE.printf("Parser script '%s' did not return table 'season'", parser->getPath().c_str());
                    return;
                }
                if (!search_params.contains("episode")) {
                    PLOGE.printf("Parser script '%s' did not return table 'episode'", parser->getPath().c_str());
                    return;
                }
                
                // If we've reached this point, we have enough information to scrape with.
                json scraper_vars;
                // Get the TV show
                json show_params = search_params["show"];
                if (show_params.contains("_tv_show_search_text")) {
                    show_search_text =
                        Library::Containers::TvShowSearchTextContainer(show_params["_tv_show_search_text"].get<std::string>());
                    show = show_search_text.getTvShow();
                    if (!show.existsInDb()) {
                        scrapeTvShow(stoken, show_params, scraper_vars, &show, parser, scraper);
                        show_search_text.scraper_vars = scraper_vars.dump();
                        show_search_text.show_title = show.title;
                        show_search_text.show_date = show.date;
                        show_search_text.writeToDb();
                    } else {
                        scraper_vars = json::parse(show_search_text.scraper_vars);
                    }
                } else {
                    PLOGW.printf("Parser script '%s' table 'show' does not contain '_tv_show_search_text'. This will force a re-scrape of the show metadata.", parser->getPath().c_str());
                    scrapeTvShow(stoken, show_params, scraper_vars, &show, parser, scraper);
                }

                episode.show_title = show.title;
                episode.show_date = show.date;

                // Get the season:
                json season_params = search_params["season"];
                Library::Containers::TvSeasonContainer season;
                season.show_title = show.title;
                season.show_date = show.date;

                if (season_params.contains("_tv_show_season_number")) {
                    season.season = season_params["_tv_show_season_number"].get<int>();
                    if (!season.existsInDb()) {
                        scrapeTvSeason(stoken, season_params, scraper_vars, &season, parser, scraper);
                    }
                } else {
                    PLOGW.printf("Parser script '%s' table 'season' does not contain '_tv_show_season_number'. This will force a re-scrape of the season metadata.", parser->getPath().c_str());
                    scrapeTvSeason(stoken, season_params, scraper_vars, &season, parser, scraper);
                }

                episode.season = season.season;

                json episode_params = search_params["episode"];
                scrapeTvEpisode(stoken, episode_params, scraper_vars, &episode, parser, scraper);
            }
        }
    }
}
void FileSource::scrapeTvShow(std::stop_token stoken,
                              json& show_params,
                              json& scraper_vars,
                              Library::Containers::TvShowContainer* show,
                              Scripting::ScriptTypes::TvShowParserScript* parser,
                              Scripting::ScriptTypes::TvShowScraperScript* scraper) {
        std::string query_results_str;
        scraper->basicShowSearch(show_params.dump(), &query_results_str);

        json query_results_json = json::parse(query_results_str);
        if (query_results_json.contains("title"))
            show->title = query_results_json["title"].get<std::string>();
        if (query_results_json.contains("date"))
            show->date = query_results_json["date"].get<std::string>();
        if (query_results_json.contains("seasons"))
            show->seasons = query_results_json["seasons"].get<int>();
        if (query_results_json.contains("desc"))
            show->desc = query_results_json["desc"].get<std::string>();
        if (query_results_json.contains("poster_path"))
            show->poster_path = query_results_json["poster_path"].get<std::string>();

        scraper_vars = query_results_json["_scraper_vars"];

        show->writeToDb();
}

void FileSource::scrapeTvSeason(std::stop_token stoken,
                    json& season_params,
                    json& scraper_vars,
                    Library::Containers::TvSeasonContainer* season,
                    Scripting::ScriptTypes::TvShowParserScript* parser,
                    Scripting::ScriptTypes::TvShowScraperScript* scraper) {
        std::string query_results_str;
        scraper->basicSeasonSearch(season_params.dump(), scraper_vars.dump(), &query_results_str);

        json query_results_json = json::parse(query_results_str);
        if (query_results_json.contains("season_number"))
            season->season = query_results_json["season_number"].get<int>();
        if (query_results_json.contains("title"))
            season->title = query_results_json["title"].get<std::string>();
        if (query_results_json.contains("desc"))
            season->desc = query_results_json["desc"].get<std::string>();
        if (query_results_json.contains("poster_path"))
            season->poster_path = query_results_json["poster_path"].get<std::string>();

        season->writeToDb();
}

void FileSource::scrapeTvEpisode(std::stop_token stoken,
                         json& season_params,
                         json& scraper_vars,
                         Library::Containers::TvEpisodeContainer* episode,
                         Scripting::ScriptTypes::TvShowParserScript* parser,
                         Scripting::ScriptTypes::TvShowScraperScript* scraper) {
    std::string query_results_str;
    scraper->basicEpisodeSearch(season_params.dump(), scraper_vars.dump(), &query_results_str);

    json query_results_json = json::parse(query_results_str);
    if (query_results_json.contains("season_number"))
        episode->season = query_results_json["season_number"].get<int>();
    if (query_results_json.contains("episode_number"))
        episode->season = query_results_json["season_number"].get<int>();
    if (query_results_json.contains("title"))
        episode->title = query_results_json["title"].get<std::string>();
    if (query_results_json.contains("air_date"))
        episode->air_date = query_results_json["air_date"].get<std::string>();
    if (query_results_json.contains("desc"))
        episode->desc = query_results_json["desc"].get<std::string>();
    if (query_results_json.contains("poster_path"))
        episode->poster_path = query_results_json["poster_path"].get<std::string>();
    
    episode->writeToDb();
}

} // namespace MediaSource::SourceType