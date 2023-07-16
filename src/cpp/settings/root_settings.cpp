#include <iostream>
#include <fstream>
#include <format>

#include <nlohmann/json.hpp>
#include <plog/Log.h>

#include "root_settings.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace Settings{

    void load_settings(RootSettings* root_settings, std::filesystem::path file_path) {
        std::ifstream in_file(file_path);
        
        if (!in_file.is_open()){
            PLOG_WARNING.printf("Settings file at '%s' could not be opened.", file_path.string().c_str());
            return;
        }

        json root_settings_json;
        in_file >> root_settings_json;
        in_file.close();

        if (root_settings_json.empty()) {
            PLOG_WARNING.printf("Settings file at '%s' was empty.", file_path.string().c_str());
            return;
        }

        // Loading settings category structs:
    
    }

    void save_settings(RootSettings* root_settings, std::filesystem::path file_path) {
        json root_settings_json;

        // Generating Json for settings category structs:

        std::ofstream out_file(file_path);
        out_file << root_settings_json.dump(4);
        out_file.close();
    }
}
