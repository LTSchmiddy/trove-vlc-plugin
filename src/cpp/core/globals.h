#pragma once

// #include "app_globals.h"
#include "core/assets/asset_globals.h"
#include "core/library/library_globals.h"
#include "core/settings/settings_globals.h"
#include "core/media_source/source_globals.h"

#ifdef UI_BUILD
    #include "app/ui/ui_globals.h"
#endif

namespace Global {
    // Set this to true to have the application exit cleanly:
    inline fs::path app_path;
}