#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <mutex>

#include "asset_wrappers/SDL_Texture_wrapper.h"
#include "core/ns_abbr/fs.h"
#include "core/gfx_headers.h"
#include "core/ns_abbr/fs.h"
#include "core/settings/settings_globals.h"

namespace Assets {
    class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        fs::path getUserHomeDirectory();
        fs::path getDataDirectory();
        fs::path getDataPath(std::string file_path);
        fs::path getAssetRoot();

        // Texture functions:
        SDL_Renderer* getTextureRenderer();
        void setTextureRenderer(SDL_Renderer* p_tex_renderer);
        
        SDL_Texture* loadTextureUnmanaged(std::string path);
        std::shared_ptr<AssetWrappers::SdlTextureWrapper> loadTexture(std::string path);
        void cleanupUnusedTextures();


    private:
        SDL_Renderer* tex_renderer = NULL;
        // Texture functions:
        std::map<std::string, std::shared_ptr<AssetWrappers::SdlTextureWrapper>> textures;
        std::recursive_mutex guard;

        void initDirectory(fs::path path);

    };
}