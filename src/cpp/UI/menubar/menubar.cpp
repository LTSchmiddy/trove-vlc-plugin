#include "menubar.h"

#include "ui/ui_globals.h"
#include "ui/dialogs/MediaSourcesDialog.h"
#include "library/library_globals.h"
#include "app_globals.h"

namespace UI {
    void draw_menu_bar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Views")) {
                if (ImGui::MenuItem("Show Media Sources Dialog")) {
                    Dialogs::create_media_sources_dialog();
                }

                if (ImGui::MenuItem("Show Demo Window", NULL, &Global::draw_demo_window));
                ImGui::EndMenu();
            }            
            ImGui::EndMainMenuBar();
        }
    }
}