#include <iostream>
#include <fstream>
#include <format>
#include <memory>

#include <plog/Log.h>

#include "root_settings.h"
#include "media_source/source_globals.h"
#include "media_source/source_types/SourceBase.h"

namespace Settings{

    void load_settings(RootSettings* root_settings, std::filesystem::path file_path) {
        std::ifstream in_file(file_path);
        
        if (!in_file.is_open()){
            // PLOG_WARNING.printf("Settings file at '%s' could not be opened.", file_path.string().c_str());
            return;
        }

        json root_settings_json;
        in_file >> root_settings_json;
        in_file.close();

        if (root_settings_json.empty()) {
            // PLOG_WARNING.printf("Settings file at '%s' was empty.", file_path.string().c_str());
            return;
        }

        // Loading settings category structs:
        if (root_settings_json.contains("log_level")) root_settings->log_level = root_settings_json["log_level"].get<int>();
        if (root_settings_json.contains("file_extensions")) root_settings->file_extensions = root_settings_json["file_extensions"].get<std::vector<std::string>>();

        if (root_settings_json.contains("scripts")) {
            json scripts_settings_json = root_settings_json["scripts"];
            if (scripts_settings_json.contains("movie_scrapers")) root_settings->scripts.movie_scrapers_path = scripts_settings_json["movie_scrapers"].get<std::string>();
            if (scripts_settings_json.contains("movie_parsers")) root_settings->scripts.movie_parsers_path = scripts_settings_json["movie_parsers"].get<std::string>();
            if (scripts_settings_json.contains("tv_scrapers")) root_settings->scripts.tv_scrapers_path = scripts_settings_json["tv_scrapers"].get<std::string>();
            if (scripts_settings_json.contains("tv_parsers")) root_settings->scripts.tv_parsers_path = scripts_settings_json["tv_parsers"].get<std::string>();
        }

        // Loading library:
        if (root_settings_json.contains("library")) {
            json library_settings_json = root_settings_json["library"];
            if (library_settings_json.contains("path")) root_settings->library.path = library_settings_json["path"].get<std::string>();
            if (library_settings_json.contains("reset_on_load")) root_settings->library.reset_on_load = library_settings_json["reset_on_load"].get<bool>();
        }

        // Loading assets:
        if (root_settings_json.contains("assets")) {
            json asset_settings_json = root_settings_json["assets"];
            if (asset_settings_json.contains("root_path")) root_settings->assets.root_path = asset_settings_json["root_path"].get<std::string>();
        }
    }

    void save_settings(RootSettings* root_settings, std::filesystem::path file_path) {
        json root_settings_json;

        // Generating Json for settings category structs:
        root_settings_json["log_level"] = root_settings->log_level;
        root_settings_json["file_extensions"] = root_settings->file_extensions;

        // Scripts settings:
        json scripts_settings_json;
        scripts_settings_json["movie_scrapers"] = root_settings->scripts.movie_scrapers_path;
        scripts_settings_json["movie_parsers"] = root_settings->scripts.movie_parsers_path;
        scripts_settings_json["tv_scrapers"] = root_settings->scripts.tv_parsers_path;
        scripts_settings_json["tv_parsers"] = root_settings->scripts.tv_parsers_path;
        root_settings_json["scripts"] = scripts_settings_json;

        // Database settings:
        json library_settings_json;
        library_settings_json["path"] = root_settings->library.path;
        library_settings_json["reset_on_load"] = root_settings->library.reset_on_load;
        root_settings_json["library"] = library_settings_json;

                // Database settings:
        json asset_settings_json;
        asset_settings_json["root_path"] = root_settings->assets.root_path;
        root_settings_json["assets"] = asset_settings_json;


        std::ofstream out_file(file_path);
        out_file << root_settings_json.dump(4);
        out_file.close();
    }
}
