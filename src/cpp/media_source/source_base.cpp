#include "source_base.h"

namespace MediaSource {

    std::string SourceBase::getType() { return "base"; }
    void SourceBase::scanForFiles() {}
    void SourceBase::loadTypeSettings(json& settings_json) {}
    void SourceBase::saveTypeSettings(json& settings_json) {}

    void SourceBase::loadSettings(json& settings_json) {
        contentType = (CONTENT_TYPE)settings_json["content_type"].get<int>();

        loadTypeSettings(settings_json);
    }
    void SourceBase::saveSettings(json& settings_json) {
        settings_json["type"] = getType();
        settings_json["content_type"] = (int)contentType;

        saveTypeSettings(settings_json);
    }
    
}