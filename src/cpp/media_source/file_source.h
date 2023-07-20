#pragma once

#include <string>
#include "ns_abbr/fs.h"

#include "source_base.h"

namespace MediaSource {
    class FileSource : public SourceBase {
    public:
        // Override functions:
        std::string getType() override;
        void loadTypeSettings(json& settings_json) override;
        void saveTypeSettings(json& settings_json) override;
        void scanForFiles() override;

        //New stuff:
        std::string rootPath;        
        fs::path getRootPath();
    private:
        void scanDirectory(fs::path dir);
    };
}