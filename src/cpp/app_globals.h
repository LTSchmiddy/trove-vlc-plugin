#pragma once

#include <vector>

#include "gfx_headers.h"
#include "ui/main_window/main_window.h"

namespace Global::App {
    // Set this to true to have the application exit cleanly:
    inline bool should_terminate = false;

    inline SDL_Window* window = NULL;
    inline SDL_Renderer* renderer = NULL;

    inline UI::MainWindow* main_window = NULL;
    
}