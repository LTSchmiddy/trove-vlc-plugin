#include "SourceBase.h"

namespace MediaSource {

    std::string SourceBase::getType() { return "base"; }
    void SourceBase::_scan(Scripting::ScriptTypes::MovieParserScript* parser) {}
    void SourceBase::loadTypeSettings(json& settings_json) {}
    void SourceBase::saveTypeSettings(json& settings_json) {}

    void SourceBase::startScan() {
        Scripting::ScriptTypes::MovieParserScript parser(parserScriptPath);
        _scan(&parser);
    }
    void SourceBase::loadSettings(json& settings_json) {
        if (settings_json.contains("content_type")) contentType = (CONTENT_TYPE)settings_json["content_type"].get<int>();
        if (settings_json.contains("parser_script_path")) parserScriptPath = settings_json["parser_script_path"].get<std::string>();

        loadTypeSettings(settings_json);
    }
    void SourceBase::saveSettings(json& settings_json) {
        settings_json["type"] = getType();
        settings_json["content_type"] = (int)contentType;
        settings_json["parser_script_path"] = parserScriptPath;

        saveTypeSettings(settings_json);
    }
    
}