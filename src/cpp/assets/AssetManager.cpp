#include <cstdlib>

#include <plog/Log.h>

#include "AssetManager.h"

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
#ifndef NDEBUG 
        return fs::current_path();
#else
        return getUserHomeDirectory().append(TROVE_DIR_NAME);
#endif
    }

    fs::path AssetManager::getDataPath(std::string file_path) {
        return getDataDirectory().append(file_path);
    }

    fs::path AssetManager::getAssetRoot() {
        return getDataPath(Global::settings.assets.root_path);
    }

    void AssetManager::initDirectory(fs::path path) {
        if (!fs::exists(path)) {
            PLOGI.printf("Directory '%s' does not exist. Creating...", path.string().c_str());
            if (!fs::create_directories(path)) {
                PLOGE.printf("Directory '%s' could not be created!", path.string().c_str());
            }
        }
    }
}