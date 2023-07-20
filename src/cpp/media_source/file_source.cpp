#include "file_source.h"

#include <plog/Log.h>

namespace MediaSource {
    std::string FileSource::getType() {
        return "file";
    }

    void FileSource::loadTypeSettings(json& settings_json) {
        if (settings_json.contains("rootPath")) rootPath = settings_json["rootPath"].get<std::string>();
    }
    void FileSource::saveTypeSettings(json& settings_json) {
        settings_json["rootPath"] = rootPath;
    }

    void FileSource::scanForFiles() {
        scanDirectory(fs::path(getRootPath()));
    }
    
    fs::path FileSource::getRootPath() {
        return fs::path(rootPath);
    }

    void FileSource::scanDirectory(fs::path dir){
        for (const fs::directory_entry &entry : fs::directory_iterator(dir)) {
            if (entry.is_directory()) {
                PLOGV.printf("Scanning Directory %s", entry.path().string().c_str());
                scanDirectory(entry);
            } else if (entry.is_regular_file()) {
                PLOGV.printf("Scanning File %s", entry.path().string().c_str());
            }
        }
    }
}