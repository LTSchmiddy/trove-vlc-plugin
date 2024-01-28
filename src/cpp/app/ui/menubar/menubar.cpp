#include "menubar.h"

#include "app/ui/ui_globals.h"
#include "app/ui/dialogs/MediaSourcesDialog.h"
#include "core/library/library_globals.h"
#include "app/app_globals.h"

namespace UI {
    void draw_menu_bar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Views")) {
                if (ImGui::MenuItem("Show Media Sources Dialog")) {
                    Dialogs::create_media_sources_dialog();
                }

                ImGui::MenuItem("Show Demo Window", NULL, &Global::draw_demo_window);
                ImGui::EndMenu();
            }            
            ImGui::EndMainMenuBar();
        }
    }
}