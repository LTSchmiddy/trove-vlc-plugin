#include "menubar.h"

#include "ui/ui_globals.h"
#include "ui/dialogs/MediaSourcesDialog.h"

namespace UI {
    void draw_menu_bar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Views")) {
                if (ImGui::MenuItem("Show Media Sources Dialog")) {
                    create_media_sources_dialog();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}