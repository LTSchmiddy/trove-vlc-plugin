#pragma once
#include <string>
#include <vector>

#include "core/ns_abbr/fs.h"
#include "core/ns_abbr/json.h"

namespace Settings {
    struct RootSettings {
        int log_level = 0;
        std::vector<std::string> file_extensions = {
            ".3gp",
            ".avi",
            ".flv",
            ".m4v",
            ".mov",
            ".mkv",
            ".mp4",
            ".mpg",
            ".nsv",
            ".ogm",
            ".ts",
            ".vid",
            ".wmv",
        };
        struct ScriptsSettings {
            std::string movie_scrapers_path = "./movie_scrapers";
            std::string movie_parsers_path = "./movie_parsers";
            std::string tv_scrapers_path = "./tv_scrapers";
            std::string tv_parsers_path = "./tv_parsers";
        } scripts;
        struct LibrarySettings {
            std::string path = "./library.db";
            bool reset_on_load = true;
        } library;
        struct AssetSettings {
            std::string root_path = "./assets";
        } assets;
    };

    void load_settings(RootSettings* settings, fs::path file);
    void save_settings(RootSettings* settings, fs::path file);
}