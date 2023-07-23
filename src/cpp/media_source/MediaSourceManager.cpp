#include <fstream>
#include <iostream>


#include <plog/Log.h>

#include "MediaSourceManager.h"
#include "source_types/FileSource.h"

namespace MediaSource {
MediaSourceManager::MediaSourceManager() {}
MediaSourceManager::~MediaSourceManager() {}

bool MediaSourceManager::renameMediaSource(std::string old_name, std::string new_name) {
    // Do not attempt if new_name already exists in media_sources;
    if (mediaSources.contains(new_name)) {
        return false;
    }

    auto nodeHandler = mediaSources.extract(old_name);
    nodeHandler.key() = new_name;
    mediaSources.insert(std::move(nodeHandler));

    return true;
}

bool MediaSourceManager::enqueueSourceUpdate(std::string name) {
    // Make sure we don't queue any source names that don't exist.
    if (mediaSources.contains(name)) {
        updateQueue.push_back(name);
        return true;
    }

    return false;
}

void MediaSourceManager::runSourceUpdates() {
    // This will be true once the list is empty:
    while (updateQueue.begin() != updateQueue.end()) {
        // Get and remove the first entry in the list:
        std::string name = updateQueue.front();
        updateQueue.pop_front();

        //Get and update the source:
        auto source = mediaSources.at(name);
        if (!source->isScanRunning()) {
            source->startScanThread(name);
            PLOGI.printf("Starting scan of media source '%s'.", name.c_str());
        }
        else {
            PLOGI.printf("Media source '%s' is already scanning.", name.c_str());
        }
    }
}

// Serialization:
void MediaSourceManager::loadSources(fs::path file_path) {
    std::ifstream in_file(file_path);

    if (!in_file.is_open()) {
        PLOG_WARNING.printf("Media source file at '%s' could not be opened.", file_path.string().c_str());
        return;
    }

    json media_sources_json;
    in_file >> media_sources_json;
    in_file.close();

    if (media_sources_json.empty()) {
        PLOG_WARNING.printf("Media source file at '%s' was empty.", file_path.string().c_str());
        return;
    }

    for (auto& it : media_sources_json.items()) {
        std::string s_type = it.value()["type"].get<std::string>();
        if (s_type == "file") {
            std::shared_ptr<MediaSource::SourceType::FileSource> new_source = std::make_shared<MediaSource::SourceType::FileSource>();
            new_source->loadSettings(it.value());
            mediaSources.emplace(it.key(), new_source);
        }
    }
}

void MediaSourceManager::saveSources(fs::path file_path) {
    // Saving media sources:
    json media_sources_json;
    for (auto& it : mediaSources) {
        json source_json;
        it.second->saveSettings(source_json);
        media_sources_json[it.first] = source_json;
    }

    std::ofstream out_file(file_path);
    out_file << media_sources_json.dump(4);
    out_file.close();
}
} // namespace MediaSource