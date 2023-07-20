#include <iostream>
#include <fstream>
#include <format>
#include <memory>

#include <plog/Log.h>

#include "root_settings.h"
#include "media_source/source_globals.h"
#include "media_source/file_source.h"

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
        if (root_settings_json.contains("log_level")) root_settings->log_level = root_settings_json["log_level"].get<int>();
        if (root_settings_json.contains("scripts_path")) root_settings->scripts_path = root_settings_json["scripts_path"].get<std::string>();
        if (root_settings_json.contains("file_extensions")) root_settings->file_extensions = root_settings_json["file_extensions"].get<std::vector<std::string>>();

        // Load media sources:
        if (root_settings_json.contains("media_sources")) {
            json media_sources_json = root_settings_json["media_sources"];

            for (auto &it : media_sources_json.items()) {
                std::string s_type = it.value()["type"].get<std::string>();
                if(s_type == "file") {
                    std::shared_ptr<MediaSource::FileSource> new_source = std::make_shared<MediaSource::FileSource>();
                    new_source->loadSettings(it.value());
                    Global::media_sources.emplace(it.key(), new_source);
                }
            }
        }
    }

    void save_settings(RootSettings* root_settings, std::filesystem::path file_path) {
        json root_settings_json;

        // Generating Json for settings category structs:
        root_settings_json["log_level"] = root_settings->log_level;
        root_settings_json["scripts_path"] = root_settings->scripts_path;
        root_settings_json["file_extensions"] = root_settings->file_extensions;

        // Saving media sources:
        json media_sources_json;
        for (auto &it : Global::media_sources) {
            json source_json;
            it.second->saveSettings(source_json);
            media_sources_json[it.first] = source_json;
        }

        root_settings_json["media_sources"] = media_sources_json;

        std::ofstream out_file(file_path);
        out_file << root_settings_json.dump(4);
        out_file.close();
    }
}
