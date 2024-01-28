#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>

#include "core/gfx_headers.h"
#include "app/ui/base/fragment.h"
#include "app/ui/main_window/MainWindow.h"
#include "app/ui/SubWindowHandler.h"

namespace Global {
    inline SDL_Window* sdl2_window = NULL;
    inline SDL_Renderer* renderer = NULL;

#ifndef CLI_BUILD
    inline UI::MainWindow* main_window = NULL;
    inline UI::SubWindowHandler* sub_window_handler = NULL;
#endif
}