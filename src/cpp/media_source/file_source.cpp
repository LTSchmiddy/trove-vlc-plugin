#include "file_source.h"

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
}