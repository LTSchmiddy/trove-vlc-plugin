#pragma once

#include <map>
#include <string>
#include <memory>
#include "source_base.h"

namespace Global {
    inline std::map<std::string, std::shared_ptr<MediaSource::SourceBase>> media_sources;
}