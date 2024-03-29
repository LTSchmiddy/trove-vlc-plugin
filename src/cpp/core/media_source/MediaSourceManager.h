#pragma once

#include <map>
#include <list>
#include <string>
#include <memory>
#include "core/ns_abbr/fs.h"

#include "source_types/SourceBase.h"


namespace MediaSource {
    class MediaSourceManager {
    public:
        MediaSourceManager();
        ~MediaSourceManager();

        bool renameMediaSource(std::string old_name, std::string new_name);
        bool enqueueSourceUpdate(std::string name);
        void runSourceUpdates();

        // Serialization;
        void loadSources(fs::path file_path);
        void saveSources(fs::path file_path);

        // Getters:
        inline std::map<std::string, std::shared_ptr<MediaSource::SourceType::SourceBase>>* getSourceMap() {
            return &mediaSources;
        }

    private:
        std::map<std::string, std::shared_ptr<MediaSource::SourceType::SourceBase>> mediaSources;
        std::list<std::string> updateQueue;
    };
}