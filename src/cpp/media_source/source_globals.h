#pragma once

#include <map>
#include <string>
#include <memory>
#include "source_types/SourceBase.h"

namespace Global {
    inline std::map<std::string, std::shared_ptr<MediaSource::SourceType::SourceBase>> media_sources;
    inline bool rename_media_source(std::string old_name, std::string new_name) {
        // Do not attempt if new_name already exists in media_sources;
        if (media_sources.contains(new_name)) {
            return false;
        }

        auto nodeHandler = media_sources.extract(old_name);
        nodeHandler.key() = new_name;
        media_sources.insert(std::move(nodeHandler));

        return true;
    }
}