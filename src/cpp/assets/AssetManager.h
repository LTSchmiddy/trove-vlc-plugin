#pragma once

#include <map>
#include <string>
#include <memory>

#include "ns_abbr/fs.h"
#include "gfx_headers.h"
#include "ns_abbr/fs.h"

#include "settings/settings_globals.h"

namespace Assets {
    class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        fs::path getUserHomeDirectory();
        fs::path getDataDirectory();
        fs::path getDataPath(std::string file_path);
        fs::path getAssetRoot();

    private:
        void initDirectory(fs::path path);
        
    };
}