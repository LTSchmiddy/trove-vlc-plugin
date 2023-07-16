#pragma once

#include "gfx_headers.h"
#include "ui/base/fragment.h"

namespace UI
{
    class MainWindow : public Fragment {
    public:
        bool open = true;
        ImGuiWindowFlags flags =  0
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoMove
        ;
        ImVec2 margins = {0.0f, 0.0f};

        MainWindow();
        ~MainWindow();

        void on_draw() override;
    };
} // namespace UI
