#include "menubar.h"

namespace UI {
    void draw_menu_bar() {
        if (ImGui::BeginMainMenuBar()) {

            ImGui::EndMainMenuBar();
        }
    }
}