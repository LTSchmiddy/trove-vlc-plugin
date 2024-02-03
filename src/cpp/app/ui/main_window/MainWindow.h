#pragma once

#include "core/ns_abbr/fs.h"
#include "core/gfx_headers.h"
#include "app/ui/base/fragment.h"

#include "app/ui/views/MovieView.h"
#include "app/ui/views/DatabaseView.h"
#include "app/ui/views/LoggerView.h"


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

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        
        
        ImVec2 margins = {0.0f, 0.0f};

        MainWindow();
        ~MainWindow();

        void onBackground() override;
        bool onEvent(SDL_Event* event) override;
        void onDraw() override;
    private:
        Views::MovieView movie_view;
        Views::DatabaseView db_view;
        Views::LoggerView logger_view;


    };
} // namespace UI
