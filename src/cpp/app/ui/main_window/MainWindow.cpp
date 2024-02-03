#include "MainWindow.h"
#include "app/app_globals.h"
#include "core/globals.h"
#include "app/ui/dialogs/MediaSourcesDialog.h"
#include "core/library/library_globals.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace UI {
    MainWindow::MainWindow() { 
        
    }
    MainWindow::~MainWindow() {
        
    }

    void MainWindow::onBackground() {
        movie_view.onBackground();
        db_view.onBackground();
        logger_view.onBackground();
    }
    bool MainWindow::onEvent(SDL_Event* event) {
        
        return movie_view.onEvent(event)
            || db_view.onEvent(event)
            || logger_view.onEvent(event)
        ;
    }

    void MainWindow::onDraw() {
        int res_x, res_y;
        SDL_GetWindowSize(Global::sdl2_window, &res_x, &res_y);

        ImVec2 window_dimens(res_x - (margins.x*2), res_y - (margins.y*2));

        ImGui::SetNextWindowPos(margins);
        ImGui::SetNextWindowSize(window_dimens);
        ImGui::Begin("primary_menu_window", NULL, flags);

        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
            if (ImGui::BeginTabItem("Movies"))
            {
                movie_view.onDraw();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Database"))
            {
                db_view.onDraw();
                ImGui::EndTabItem();
            }
                if (ImGui::BeginTabItem("Log"))
            {
                logger_view.onDraw();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        

        ImGui::End();
    }
}