#include "main_window.h"
#include "app_globals.h"

namespace UI {
    MainWindow::MainWindow() { }
    MainWindow::~MainWindow() { }

    void MainWindow::on_draw() {

        int res_x, res_y;

        SDL_GetWindowSize(Global::App::window, &res_x, &res_y);

        ImVec2 menu_dimens(res_x - (margins.x*2), res_y - (margins.y*2));

        ImGui::SetNextWindowPos(margins);
        ImGui::SetNextWindowSize(menu_dimens);
        ImGui::Begin("primary_menu_window", NULL, flags);
        

        ImGui::End();
    }
}