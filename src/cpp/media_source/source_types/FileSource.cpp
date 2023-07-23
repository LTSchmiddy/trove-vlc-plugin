#include <algorithm>

#include <plog/Log.h>

#include "FileSource.h"
#include "settings/settings_globals.h"
#include "ns_abbr/json.h"
#include "library/containers/MovieContainer.h"


namespace MediaSource::SourceType {
    std::string FileSource::getType() {
        return "file";
    }

    void FileSource::loadTypeSettings(json& settings_json) {
        if (settings_json.contains("rootPath")) rootPath = settings_json["rootPath"].get<std::string>();
    }
    void FileSource::saveTypeSettings(json& settings_json) {
        settings_json["rootPath"] = rootPath;
    }

    fs::path FileSource::getRootPath() {
        return fs::path(rootPath);
    }

    void FileSource::scan(Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper) {
        scanDirectory(fs::path(getRootPath()), parser, scraper);
    }

    void FileSource::scanDirectory(fs::path dir, Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper){
        for (const fs::directory_entry &entry : fs::directory_iterator(dir)) {
            if (entry.is_directory()) {
                PLOGV.printf("Scanning Directory %s", entry.path().string().c_str());
                scanDirectory(entry, parser, scraper);
            } else if (entry.is_regular_file()) {
                PLOGV.printf("Scanning File %s", entry.path().string().c_str());
                
                // Skip this file if it's not a usable extension:
                if(std::find(
                    Global::settings.file_extensions.begin(),
                    Global::settings.file_extensions.end(),
                    entry.path().extension().string()
                ) == Global::settings.file_extensions.end()) {
                    continue;
                }

                // We're still operating on relative paths:
                fs::path rel_path = fs::relative(entry.path(), getRootPath());
                Library::Containers::MovieContainer new_movie = Library::Containers::MovieContainer();

                new_movie.source = sourceName;
                new_movie.path = rel_path.string().c_str();
                
                // This will enable us to skip movies we've already scanned
                if (!new_movie.existsInDb()) {
                    std::string search_params = parser->parsePath(rel_path.parent_path().string(), rel_path.filename().string());

                    std::string query_results_str;
                    scraper->basicSearch(search_params, &query_results_str);

                    PLOGD << query_results_str;
                    json query_results_json = json::parse(query_results_str);

                    if (query_results_json.contains("title")) new_movie.title = query_results_json["title"].get<std::string>();
                    if (query_results_json.contains("date")) new_movie.date = query_results_json["date"].get<std::string>();
                    if (query_results_json.contains("desc")) new_movie.desc = query_results_json["desc"].get<std::string>();

                    new_movie.writeToDb();
                }
            }
        }
    }
}