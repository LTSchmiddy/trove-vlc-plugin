#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>

#include "gfx_headers.h"
#include "ui/base/fragment.h"
#include "ui/main_window/MainWindow.h"
#include "ui/SubWindowHandler.h"

namespace Global {
    inline SDL_Window* sdl2_window = NULL;
    inline SDL_Renderer* renderer = NULL;

#ifndef CLI_BUILD
    inline UI::MainWindow* main_window = NULL;
    inline UI::SubWindowHandler* sub_window_handler = NULL;
#endif
}