#pragma once

#include "ns_abbr/fs.h"
#include "gfx_headers.h"
#include "ui/base/fragment.h"

#include "ui/views/MovieView.h"

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

        void onBackground() override;
        bool onEvent(SDL_Event* event) override;
        void onDraw() override;
    private:
        Views::MovieView movie_view;


    };
} // namespace UI
