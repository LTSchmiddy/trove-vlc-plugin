#pragma once

#include <string>
#include "ns_abbr/fs.h"

#include "SourceBase.h"

namespace MediaSource::SourceType {
    class FileSource : public SourceBase {
    public:
        // Override functions:
        std::string getType() override;
        void loadTypeSettings(json& settings_json) override;
        void saveTypeSettings(json& settings_json) override;
        void _scan(Scripting::ScriptTypes::MovieParserScript* parser) override;

        //New stuff:
        std::string rootPath;
        fs::path getRootPath();
    private:
        void scanDirectory(fs::path dir, Scripting::ScriptTypes::MovieParserScript* parser);
    };
}