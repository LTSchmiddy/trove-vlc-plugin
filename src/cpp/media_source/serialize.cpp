#include <fstream>
#include <plog/Log.h>

#include "serialize.h"

#include "file_source.h"

namespace MediaSource {
    void load_sources(std::map<std::string, std::shared_ptr<MediaSource::SourceBase>>* media_sources, fs::path file_path) {
        std::ifstream in_file(file_path);
        
        if (!in_file.is_open()){
            PLOG_WARNING.printf("Media source file at '%s' could not be opened.", file_path.string().c_str());
            return;
        }

        json media_sources_json;
        in_file >> media_sources_json;
        in_file.close();

        if (media_sources_json.empty()) {
            PLOG_WARNING.printf("Media source file at '%s' was empty.", file_path.string().c_str());
            return;
        }

        for (auto &it : media_sources_json.items()) {
            std::string s_type = it.value()["type"].get<std::string>();
            if(s_type == "file") {
                std::shared_ptr<MediaSource::FileSource> new_source = std::make_shared<MediaSource::FileSource>();
                new_source->loadSettings(it.value());
                media_sources->emplace(it.key(), new_source);
            }
        }
    }
    
    void save_sources(std::map<std::string, std::shared_ptr<MediaSource::SourceBase>>* media_sources, fs::path file_path) {
        // Saving media sources:
        json media_sources_json;
        for (auto &it : *media_sources) {
            json source_json;
            it.second->saveSettings(source_json);
            media_sources_json[it.first] = source_json;
        }

        std::ofstream out_file(file_path);
        out_file << media_sources_json.dump(4);
        out_file.close();
    }
}