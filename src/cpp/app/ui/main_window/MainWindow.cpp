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
    }
    bool MainWindow::onEvent(SDL_Event* event) {
        
        return movie_view.onEvent(event);
    }

    void MainWindow::onDraw() {
        int res_x, res_y;
        SDL_GetWindowSize(Global::sdl2_window, &res_x, &res_y);

        ImVec2 window_dimens(res_x - (margins.x*2), res_y - (margins.y*2));

        ImGui::SetNextWindowPos(margins);
        ImGui::SetNextWindowSize(window_dimens);
        ImGui::Begin("primary_menu_window", NULL, flags);

        movie_view.onDraw();

        ImGui::End();
    }
}