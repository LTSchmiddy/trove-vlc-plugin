#include "SourceBase.h"
#include <plog/Log.h>

namespace MediaSource::SourceType {

    std::string SourceBase::getType() { return "base"; }
    void SourceBase::scan(Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper) {}
    void SourceBase::loadTypeSettings(json& settings_json) {}
    void SourceBase::saveTypeSettings(json& settings_json) {}

    void SourceBase::loadSettings(json& settings_json) {
        if (settings_json.contains("content_type")) contentType = (CONTENT_TYPE)settings_json["content_type"].get<int>();
        if (settings_json.contains("parser_script_path")) parserScriptPath = settings_json["parser_script_path"].get<std::string>();
        if (settings_json.contains("scraper_script_path")) scraperScriptPath = settings_json["scraper_script_path"].get<std::string>();

        loadTypeSettings(settings_json);
    }
    void SourceBase::saveSettings(json& settings_json) {
        settings_json["type"] = getType();
        settings_json["content_type"] = (int)contentType;
        settings_json["parser_script_path"] = parserScriptPath;
        settings_json["scraper_script_path"] = scraperScriptPath;

        saveTypeSettings(settings_json);
    }

    void SourceBase::startScanThread() {
        if (scanThread.joinable()) {
            scanThread.join();
        }
        scanThread = std::thread([this](){
            // Indicate the scan is running:
            this->_isScanRunning = true;
            
            // Load parser and scraper:
            Scripting::ScriptTypes::MovieParserScript parser(parserScriptPath);
            Scripting::ScriptTypes::MovieScraperScript scraper(scraperScriptPath);

            std::lock_guard lock(this->guard);
            this->scan(&parser, &scraper);

            // Scan is over:
            this->_isScanRunning = false;
        });
    }

    // void SourceBase::scrape(std::string query, Scripting::ScriptTypes::MovieScraperScript* scraper) {
    //     PLOGD << query;
    // }
}