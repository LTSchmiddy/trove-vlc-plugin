#pragma once

#include <string>
#include "ns_abbr/fs.h"

#include "SourceBase.h"

namespace MediaSource::SourceType {
    class FileSource : public SourceBase {
    public:
        // Override functions:
        std::string getType() override;
        std::string getUriPrefix() override;
        void loadTypeSettings(json& settings_json) override;
        void saveTypeSettings(json& settings_json) override;
        void scan(std::stop_token stoken, std::string name, Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper) override;

        SOURCE_GETSET(std::string, _rootPath);
    private:
        std::string _rootPath;
        void scanDirectory(std::stop_token stoken, std::string name, fs::path dir, Scripting::ScriptTypes::MovieParserScript* parser, Scripting::ScriptTypes::MovieScraperScript* scraper);
    };
}