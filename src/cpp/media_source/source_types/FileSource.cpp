#include <algorithm>

#include <plog/Log.h>

#include "FileSource.h"
#include "library/containers/MovieContainer.h"
#include "library/containers/TvShowContainer.h"
#include "library/containers/TvSeasonContainer.h"
#include "library/containers/TvEpisodeContainer.h"
#include "ns_abbr/json.h"
#include "settings/settings_globals.h"

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
            PLOGV.printf("Scanning Movie File %s", entry.path().string().c_str());

            // Skip this file if it's not a usable extension:
            if (std::find(Global::settings.file_extensions.begin(),
                          Global::settings.file_extensions.end(),
                          entry.path().extension().string()) == Global::settings.file_extensions.end()) {
                continue;
            }

            // We're still operating on relative paths:
            fs::path rel_path = fs::relative(entry.path(), get_rootPath());
            Library::Containers::MovieContainer new_movie = Library::Containers::MovieContainer(src_name, rel_path.string(), false);

            // Check for a stop again before we do any db access:
            if (stoken.stop_requested()) {
                return;
            }

            // This will enable us to skip movies we've already scanned
            if (!new_movie.existsInDb()) {
                std::string search_params = parser->parsePath(rel_path.parent_path().string(), rel_path.filename().string());
                // std::string search_params = parser->parsePath("Batman Begins (2005)", "what.mp4");

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
            }
        }
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

            // Loading a database container for the file:
            Library::Containers::TvEpisodeContainer episode(src_name, rel_path.string(), false); // We don't need to look up the stored episode data right now.


                        // Check for a stop again before we do any db access:
            if (stoken.stop_requested()) {
                return;
            }

            // No need to do anything if the file is already in the db.
            if (!episode.existsInDb()) {
                // Parse file path for search info:
                std::string search_params = parser->parsePath(rel_path.parent_path().string(), rel_path.filename().string());
                // Check for a stop again after scraping (expecially since scraping requires internet access and can be time consuming):
                if (stoken.stop_requested()) {
                    return;
                }
                std::string query_results_str;
                scraper->basicEpisodeSearch(search_params, &query_results_str);
            }
        }
    }
}
} // namespace MediaSource::SourceType