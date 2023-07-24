#include <plog/Log.h>

#include "AssetManager.h"


namespace Assets {
    AssetManager::AssetManager() {
        initDirectory(getAssetRoot());
        // initDirectory(getMoviePosterRoot());
    }
    AssetManager::~AssetManager() {}

    void AssetManager::initDirectory(fs::path path) {
        if (!fs::exists(path)) {
            PLOGI.printf("Asset root '%s' does not exist. Creating...", path.string().c_str());
            fs::create_directories(path);
        }
    }
}