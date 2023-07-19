#include "main_window.h"
#include "globals.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace UI {
    MainWindow::MainWindow() { 
        script_dir = fs::path(Global::settings.scripts_path);
    }
    MainWindow::~MainWindow() {
        if (scraper == NULL) {
            delete scraper;
        }
    }

    void MainWindow::onDraw() {

        int res_x, res_y;

        SDL_GetWindowSize(Global::App::window, &res_x, &res_y);

        ImVec2 menu_dimens(res_x - (margins.x*2), res_y - (margins.y*2));

        ImGui::SetNextWindowPos(margins);
        ImGui::SetNextWindowSize(menu_dimens);
        ImGui::Begin("primary_menu_window", NULL, flags);
        
        for (const fs::directory_entry &entry : fs::directory_iterator(script_dir)) {
            if (scraper == NULL) {
                ImGui::Text("Select Scraper:");
                if (ImGui::Button(entry.path().string().c_str())) {
                    scraper = new Scripting::ScraperScript(entry.path());
                    
                }
            } else if (!scraper->isLoaded()) {
                ImGui::Text("There was an error in loading this scraper");
                ImGui::SameLine();
                if (ImGui::Button("Close")) {
                    delete scraper;
                    scraper = NULL;
                }
            } else {
                ImGui::Text("Scraper Loaded.");
                ImGui::SameLine();
                if (ImGui::Button("Close")) {
                    delete scraper;
                    scraper = NULL;
                } else {
                    drawQueryInterface();
                }
            }
        }
        ImGui::End();
    }

    void MainWindow::drawQueryInterface() {
        bool search_box = ImGui::InputText("##basicSearchBox", &basicSearchParam, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::SameLine();
        bool search_btn = ImGui::Button("Search##basicSearchButton");

        ImVec2 out_size = ImGui::GetWindowSize();
        out_size.y -= 100.0f;

        ImGui::InputTextMultiline("Result", &scrapOutput, out_size, ImGuiInputTextFlags_ReadOnly);

        if (search_box || search_btn) {
            scraper->basicSearch(basicSearchParam, &scrapOutput);

            // Prettyify the output:
            scrapOutput = json::parse(scrapOutput).dump(4);
        }
    }
}