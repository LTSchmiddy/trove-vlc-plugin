#pragma once
#include <string>

#include "fs.h"

namespace Settings {
    struct RootSettings {
        std::string scripts_path = "./";
    };

    void load_settings(RootSettings* settings, fs::path file);
    void save_settings(RootSettings* settings, fs::path file);
}