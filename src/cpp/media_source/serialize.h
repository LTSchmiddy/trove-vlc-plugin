#include <map>
#include <string>
#include <memory>

#include "ns_abbr/fs.h"
#include "ns_abbr/json.h"
#include "source_base.h"

namespace MediaSource {
    void load_sources(std::map<std::string, std::shared_ptr<MediaSource::SourceBase>>* media_sources, fs::path file_path);
    void save_sources(std::map<std::string, std::shared_ptr<MediaSource::SourceBase>>* media_sources, fs::path file_path);
}