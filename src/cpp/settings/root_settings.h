#pragma once
#include <string>
#include <filesystem>

namespace Settings {
    struct RootSettings {

    };

    void load_settings(RootSettings* settings, std::filesystem::path file);
    void save_settings(RootSettings* settings, std::filesystem::path file);
}