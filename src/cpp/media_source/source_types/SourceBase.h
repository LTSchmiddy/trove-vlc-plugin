#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <future>

#include "ns_abbr/json.h"
#include "scripting/script_types/MovieParserScript.h"
#include "scripting/script_types/MovieScraperScript.h"

#define SOURCE_GETTER_FUNC(VAR_TYPE, VAR_NAME) \
    inline VAR_TYPE get##VAR_NAME() { \
        return VAR_NAME; \
    }

#define SOURCE_SETTER_FUNC(VAR_TYPE, VAR_NAME) \
    inline void set##VAR_NAME(VAR_TYPE val) { \
        std::lock_guard lock(guard); \
        VAR_NAME = val; \
    }

#define SOURCE_GETSET(VAR_TYPE, VAR_NAME) \
    SOURCE_GETTER_FUNC(VAR_TYPE, VAR_NAME) \
    SOURCE_SETTER_FUNC(VAR_TYPE, VAR_NAME)

namespace MediaSource::SourceType {
    enum class CONTENT_TYPE {
        MOVIES = 0,
        TV_SHOWS = 1,
        OTHER = 2,
    };

    // Because I need sources to be thread-safe, all variable access
    // needs to be done with mutex-locked getters and setters
    class SourceBase {
    public:
        void loadSettings(json& settings_json);
        void saveSettings(json& settings_json);
        void startScanThread(std::string name);
        bool stopScanThread();

        virtual std::string getType();
        virtual std::string getUriPrefix();

        inline bool isScanRunning() {
            return _isScanRunning;
        }

        // Using a macro to write out the getters and setters. 
        // Not something I normally support, but...
        SOURCE_GETSET(CONTENT_TYPE, _contentType);
        SOURCE_GETSET(std::string, _parserScriptPath);
        SOURCE_GETSET(std::string , _scraperScriptPath);

    protected:
        // Public info, uses getters/setters:
        CONTENT_TYPE _contentType = CONTENT_TYPE::MOVIES;
        std::string _parserScriptPath;
        std::string _scraperScriptPath;
        
        std::recursive_mutex guard;
        std::jthread scanThread;
        bool _isScanRunning = false;

        virtual void scan(std::stop_token stoken, std::string name, Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper);
        virtual void loadTypeSettings(json& settings_json);
        virtual void saveTypeSettings(json& settings_json);
    };
}