#include <memory>

#include "MediaSourcesDialog.h"
#include "ui/ui_globals.h"

namespace UI {

    void create_media_sources_dialog() {
        Global::sub_window_handler->addSubWindow("media_souces",
            std::make_shared<FWindow>("Media Sources##media_sources", new MediaSourcesDialog(), true, true)
        );
    }
}