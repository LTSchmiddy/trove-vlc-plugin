#include "main_window.h"
#include "globals.h"
#include "ui/dialogs/MediaSourcesDialog.h"

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

        SDL_GetWindowSize(Global::sdl2_window, &res_x, &res_y);

        ImVec2 window_dimens(res_x - (margins.x*2), res_y - (margins.y*2));

        ImGui::SetNextWindowPos(margins);
        ImGui::SetNextWindowSize(window_dimens);
        ImGui::Begin("primary_menu_window", NULL, flags);

        ImGui::End();
    }

    void MainWindow::drawQueryInterface() {
        // If no scraper is loaded:
        if (scraper == NULL) {
            ImGui::Text("Select Scraper:");
            for (const fs::directory_entry &entry : fs::directory_iterator(script_dir)) {
                if (ImGui::Button(entry.path().string().c_str())) {
                    scraper = new Scripting::MovieScraperScript(entry.path());
                }
            }
        // If a scraper failed to start:
        } else if (!scraper->isLoaded()) {
            ImGui::Text("There was an error in loading this scraper. Check the log for details.");
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                delete scraper;
                scraper = NULL;
            }
        // Scraper is loaded:
        } else {
            ImGui::Text("Scraper Loaded.");
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                delete scraper;
                scraper = NULL;
            } else {
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
    }
}