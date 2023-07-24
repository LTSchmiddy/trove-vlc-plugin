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

        fs::path getAssetRoot() { return fs::path(Global::settings.assets.root_path); }
        // fs::path getMoviePosterRoot() { return getAssetRoot().append("movie_posters"); }

    private:
        void initDirectory(fs::path path);
        
    };
}