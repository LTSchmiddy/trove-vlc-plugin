#pragma once

#include <string>
#include <vector>
#include "ns_abbr/json.h"
#include "scripting/script_types/MovieParserScript.h"

namespace MediaSource {
    enum class CONTENT_TYPE {
        MOVIES = 0,
        TV_SHOWS = 1,
        OTHER = 2,
    };

    class SourceBase {
    public:
        CONTENT_TYPE contentType = CONTENT_TYPE::MOVIES;
        std::string parserScriptPath;

        void loadSettings(json& settings_json);
        void saveSettings(json& settings_json);
        void startScan();

        virtual std::string getType();
        
    private:
        virtual void _scan(Scripting::ScriptTypes::MovieParserScript* parser);
        virtual void loadTypeSettings(json& settings_json);
        virtual void saveTypeSettings(json& settings_json);
    };
}