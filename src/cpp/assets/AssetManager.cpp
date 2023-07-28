#include <cstdlib>
#include <list>

#include <plog/Log.h>

#include "AssetManager.h"

#ifdef UI_BUILD
    #include "ui/ui_globals.h"
#endif

#define TROVE_DIR_NAME ".trove"

namespace Assets {
    AssetManager::AssetManager() {
        PLOGI.printf("HOME: %s", getDataDirectory().string().c_str());

        initDirectory(getDataDirectory());
        initDirectory(getAssetRoot());
    }
    AssetManager::~AssetManager() {}

    fs::path AssetManager::getUserHomeDirectory() {
#ifdef _WIN32

    char* hdrive_value;
    size_t hdrive_len;
    errno_t hdrive_err = _dupenv_s(&hdrive_value, &hdrive_len, "HOMEDRIVE");

    char* hpath_value;
    size_t hpath_len;
    errno_t hpath_err = _dupenv_s(&hpath_value, &hpath_len, "HOMEPATH");

    return fs::path(std::string(hdrive_value) + std::string(hpath_value));
#else
    return fs::path(std::getenv("HOME"));
#endif
    }

    fs::path AssetManager::getDataDirectory() {
        std::lock_guard lock(guard);
#ifndef NDEBUG 
        return fs::current_path();
#else
        return getUserHomeDirectory().append(TROVE_DIR_NAME);
#endif
    }

    fs::path AssetManager::getDataPath(std::string file_path) {
        std::lock_guard lock(guard);
        return getDataDirectory().append(file_path);
    }

    fs::path AssetManager::getAssetRoot() {
        std::lock_guard lock(guard);
        return getDataPath(Global::settings.assets.root_path);
    }

    void AssetManager::initDirectory(fs::path path) {
        std::lock_guard lock(guard);
        if (!fs::exists(path)) {
            PLOGI.printf("Directory '%s' does not exist. Creating...", path.string().c_str());
            if (!fs::create_directories(path)) {
                PLOGE.printf("Directory '%s' could not be created!", path.string().c_str());
            }
        }
    }

#ifdef UI_BUILD
    SDL_Texture* AssetManager::loadTextureUnmanaged(std::string path) {
        std::lock_guard lock(guard);
        SDL_Texture* retVal = NULL;
        
        fs::path fs_path(path);
        if (fs_path.is_absolute()) {
            retVal = IMG_LoadTexture(Global::renderer, path.c_str());
        } else {
            retVal = IMG_LoadTexture(Global::renderer, getDataPath(path).string().c_str());
        }

        if (retVal == NULL){
            PLOG_ERROR << std::format("Error in {} line {}: could not load Texture at path '{}'\n", __FILE__, __LINE__, path);
        }

        return retVal;
    }

    std::shared_ptr<AssetWrappers::SdlTextureWrapper> AssetManager::loadTexture(std::string path) {
        std::lock_guard lock(guard);
        if (textures.contains(path)) {
            // Return previously loaded instance:
            PLOGV << std::format("Texture at path '{}' has been loaded {} times.", path, textures.at(path).use_count());
            return textures.at(path);
        }
        // Load from scratch
        std::shared_ptr<AssetWrappers::SdlTextureWrapper> wrapper = std::make_shared<AssetWrappers::SdlTextureWrapper>(loadTextureUnmanaged(path));
        textures.insert_or_assign(path, wrapper);


        PLOG_DEBUG << std::format("Loading texture at path '{}'", path);
        return wrapper;
    }

    void AssetManager::cleanupUnusedTextures() {
        std::lock_guard lock(guard);
        // ===== Textures =====
        // Find textures to remove:        
        std::list<std::string> textures_to_remove;
        for (std::map<std::string, std::shared_ptr<AssetWrappers::SdlTextureWrapper>>::iterator it = textures.begin(); it != textures.end(); it++) {
            // If the use_count is less than or equal to 1, then the only place the asset exists is in the asset manager. 
            // Hence, we should get rid of it.
            if (it->second.use_count() <= 1) {
                textures_to_remove.push_back(it->first);
            }
        }

        // Actually remove them.
        for (std::string path : textures_to_remove) {
            // We don't need to call delete on the asset, since the shared pointer will handle that itself.
            PLOG_DEBUG << std::format("Deleting texture at path '{}", path);
            textures.erase(path);
        }
    }
#endif

}