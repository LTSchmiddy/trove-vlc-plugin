#pragma once

#include <memory>

#include "ui/base/fragment.h"
#include "media_source/source_types/FileSource.h"

namespace UI {
    const std::string contentTypeLabels[] = {"Movies", "TV Shows", "Other"};

    class MediaSourcesDialog : public Fragment {
    public:
        void onDraw() override;

    private:
        void drawMenuBar();
        void drawLocations();
        void drawFileLocation(MediaSource::SourceType::FileSource* source);
    };

    void create_media_sources_dialog();
}