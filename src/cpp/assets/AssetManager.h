#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <mutex>

#include "asset_wrappers/SDL_Texture_wrapper.h"
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

#ifdef UI_BUILD
        // Texture functions:
        SDL_Texture* loadTextureUnmanaged(std::string path);
        std::shared_ptr<AssetWrappers::SdlTextureWrapper> loadTexture(std::string path);
        void cleanupUnusedTextures();
#endif

    private:
#ifdef UI_BUILD
        // Texture functions:
        std::map<std::string, std::shared_ptr<AssetWrappers::SdlTextureWrapper>> textures;
#endif
        std::recursive_mutex guard;

        void initDirectory(fs::path path);

    };
}