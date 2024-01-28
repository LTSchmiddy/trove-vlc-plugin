#pragma once

#include <memory>

#include "app/ui/base/fragment.h"
#include "core/media_source/source_types/FileSource.h"

namespace UI::Dialogs {
    const std::string contentTypeLabels[] = {"Movies", "TV Shows", "Other"};

    class MediaSourcesDialog : public Fragment {
    public:
        void onDraw() override;

    private:
        void drawMenuBar();
        void drawLocations();
        void drawFileSource(MediaSource::SourceType::FileSource* source);
    };

    void create_media_sources_dialog();
}