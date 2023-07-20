#pragma once
#include <string>
#include <vector>

#include "ns_abbr/fs.h"
#include "ns_abbr/json.h"

namespace Settings {
    struct RootSettings {
        std::string scripts_path = "./";
        std::vector<std::string> file_extensions = {
            ".mp4",
            ".m4v",
            ".avi"
        };
    };

    void load_settings(RootSettings* settings, fs::path file);
    void save_settings(RootSettings* settings, fs::path file);
}