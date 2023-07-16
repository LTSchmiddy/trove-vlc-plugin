
#include "fragment.h"

namespace UI {
// === Fragment Class Defs ===

bool Fragment::on_event(SDL_Event* event) {
    return false;
}
void Fragment::on_background() { }
void Fragment::on_draw() { }

// === FWindow Class Defs ===
// Constructor and destructor:
FWindow::FWindow(std::string p_id, Fragment* p_content, bool p_destruct_content, bool p_start_open, ImGuiWindowFlags p_flags) {
    id = p_id;
    open = p_start_open;
    flags = p_flags;
    content = p_content;
    destruct_content = p_destruct_content;
}

FWindow::~FWindow() {
    // If we're responsible for destroying the content fragment, do that now:
    if (destruct_content) {
        delete content;
    }
}

// Override Methods:
bool FWindow::on_event(SDL_Event* event) {
    return content->on_event(event);
}

void FWindow::on_background() {
    content->on_background();
}

void FWindow::on_draw() {
    ImGui::Begin(id.c_str(), &open, flags);
    content->on_draw();
    ImGui::End();
}
}