#include "SourceBase.h"
#include <plog/Log.h>

namespace MediaSource::SourceType {

    std::string SourceBase::getType() { return "base"; }
    void SourceBase::scan(std::stop_token stoken, std::string name, Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper) {}
    void SourceBase::loadTypeSettings(json& settings_json) {}
    void SourceBase::saveTypeSettings(json& settings_json) {}

    void SourceBase::loadSettings(json& settings_json) {
        if (settings_json.contains("content_type")) _contentType = (CONTENT_TYPE)settings_json["content_type"].get<int>();
        if (settings_json.contains("parser_script_path")) _parserScriptPath = settings_json["parser_script_path"].get<std::string>();
        if (settings_json.contains("scraper_script_path")) _scraperScriptPath = settings_json["scraper_script_path"].get<std::string>();

        loadTypeSettings(settings_json);
    }
    void SourceBase::saveSettings(json& settings_json) {
        settings_json["type"] = getType();
        settings_json["content_type"] = (int)_contentType;
        settings_json["parser_script_path"] = _parserScriptPath;
        settings_json["scraper_script_path"] = _scraperScriptPath;

        saveTypeSettings(settings_json);
    }

    void SourceBase::startScanThread(std::string name) {
        if (scanThread.joinable()) {
            scanThread.join();
        }
        scanThread = std::jthread([this, name](std::stop_token stoken){
            // Indicate the scan is running:
            this->_isScanRunning = true;
            
            // Load parser and scraper:
            Scripting::ScriptTypes::MovieParserScript parser(_parserScriptPath);
            Scripting::ScriptTypes::MovieScraperScript scraper(_scraperScriptPath);
            this->scan(stoken, name, &parser, &scraper);

            // Scan is over:
            this->_isScanRunning = false;
        });
    }
    bool SourceBase::stopScanThread() {
        return scanThread.request_stop();
    }
}