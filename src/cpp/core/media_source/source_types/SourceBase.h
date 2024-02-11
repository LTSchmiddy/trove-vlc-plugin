#pragma once

#include <string>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <future>

#include "core/ns_abbr/json.h"
#include "core/scripting/script_types/MovieParserScript.h"
#include "core/scripting/script_types/MovieScraperScript.h"
#include "core/scripting/script_types/TvShowParserScript.h"
#include "core/scripting/script_types/TvShowScraperScript.h"

#define SOURCE_GETTER_FUNC(VAR_TYPE, VAR_NAME) \
    inline VAR_TYPE get##VAR_NAME() { \
        return VAR_NAME; \
    }

#define SOURCE_SETTER_FUNC(VAR_TYPE, VAR_NAME) \
    inline void set##VAR_NAME(VAR_TYPE val) { \
        std::lock_guard lock(scanGuard); \
        VAR_NAME = val; \
    }

#define SOURCE_GETSET(VAR_TYPE, VAR_NAME) \
    SOURCE_GETTER_FUNC(VAR_TYPE, VAR_NAME) \
    SOURCE_SETTER_FUNC(VAR_TYPE, VAR_NAME)

#define PROGRESS_GETTER_FUNC(VAR_TYPE, VAR_NAME) \
    inline VAR_TYPE get##VAR_NAME() { \
        std::lock_guard lock(progressGuard); \
        return VAR_NAME; \
    }

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
        PROGRESS_GETTER_FUNC(float, _scanProgress);
        PROGRESS_GETTER_FUNC(std::string, _currentScanPath);

    protected:
        // Public info, uses getters/setters:
        CONTENT_TYPE _contentType = CONTENT_TYPE::MOVIES;
        std::string _parserScriptPath;
        std::string _scraperScriptPath;
        
        std::recursive_mutex scanGuard;
        std::jthread scanThread;
        bool _isScanRunning = false;

        std::recursive_mutex progressGuard;
        float _scanProgress = 0.0f;
        std::string _currentScanPath;

        virtual void scanForMovies(std::stop_token stoken, std::string name, Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper);
        virtual void scanForTvShows(std::stop_token stoken, std::string name, Scripting::ScriptTypes::TvShowParserScript* parser, Scripting::ScriptTypes::TvShowScraperScript* scraper);
        virtual void loadTypeSettings(json& settings_json);
        virtual void saveTypeSettings(json& settings_json);
    };
}