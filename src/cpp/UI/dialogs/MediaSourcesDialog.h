#pragma once

#include <memory>

#include "ui/base/fragment.h"
#include "media_source/file_source.h"

namespace UI {
    const std::string contentTypeLabels[] = {"Movies", "TV Shows", "Other"};

    class MediaSourcesDialog : public Fragment {
    public:
        void onDraw() override;

    private:
        void drawMenuBar();
        void drawLocations();
        void drawFileLocation(MediaSource::FileSource* source);
    };

    void create_media_sources_dialog();
}