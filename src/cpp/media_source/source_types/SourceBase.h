#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <future>

#include "ns_abbr/json.h"
#include "scripting/script_types/MovieParserScript.h"
#include "scripting/script_types/MovieScraperScript.h"

namespace MediaSource::SourceType {
    enum class CONTENT_TYPE {
        MOVIES = 0,
        TV_SHOWS = 1,
        OTHER = 2,
    };

    class SourceBase {
    public:
        CONTENT_TYPE contentType = CONTENT_TYPE::MOVIES;
        std::string parserScriptPath;
        std::string scraperScriptPath;
        std::string sourceName;
        std::recursive_mutex guard;

        void loadSettings(json& settings_json);
        void saveSettings(json& settings_json);
        void startScanThread();

        virtual std::string getType();

        inline bool isScanRunning() {
            return _isScanRunning;
        }
        
    protected:
        std::thread scanThread;
        bool _isScanRunning = false;
        // void scrape(std::string query, Scripting::ScriptTypes::MovieScraperScript* scraper);

        virtual void scan(Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper);
        virtual void loadTypeSettings(json& settings_json);
        virtual void saveTypeSettings(json& settings_json);
    };
}