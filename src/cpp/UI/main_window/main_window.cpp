#include "main_window.h"
#include "globals.h"

#include "scripting/scraper_script.h"

namespace fs = fs;
namespace UI {
    MainWindow::MainWindow() { 
        script_dir = fs::path(Global::settings.scripts_path);
    }
    MainWindow::~MainWindow() { }

    void MainWindow::onDraw() {

        int res_x, res_y;

        SDL_GetWindowSize(Global::App::window, &res_x, &res_y);

        ImVec2 menu_dimens(res_x - (margins.x*2), res_y - (margins.y*2));

        ImGui::SetNextWindowPos(margins);
        ImGui::SetNextWindowSize(menu_dimens);
        ImGui::Begin("primary_menu_window", NULL, flags);
        
        for (const fs::directory_entry &entry : fs::directory_iterator(script_dir)) {
            // ImGui::Text("%s", entry.path().string().c_str());
            if (ImGui::Button(entry.path().string().c_str())) {
                Scripting::ScraperScript script(entry.path());
            }
        }
        ImGui::End();
    }
}