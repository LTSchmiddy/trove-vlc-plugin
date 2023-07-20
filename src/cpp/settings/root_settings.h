#pragma once
#include <string>
#include <vector>

#include "ns_abbr/fs.h"
#include "ns_abbr/json.h"

namespace Settings {
    struct RootSettings {
        int log_level = 0;
        std::string scripts_path = "./";
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
        struct LibrarySettings {
            std::string path = "./library.db";
            bool reset = true;
        } library;
    };

    void load_settings(RootSettings* settings, fs::path file);
    void save_settings(RootSettings* settings, fs::path file);
}