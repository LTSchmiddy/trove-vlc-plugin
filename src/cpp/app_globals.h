#pragma once

#include "ns_abbr/fs.h"

namespace Global {
    // Set this to true to have the application exit cleanly:
    inline fs::path app_path;
    inline bool should_terminate = false;
    inline bool draw_demo_window = false;
}