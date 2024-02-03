#include <sqlite3.h>

#include "DatabaseView.h"

namespace UI::Views {
    DatabaseView::DatabaseView(){

    }
    DatabaseView::~DatabaseView(){}

    void DatabaseView::onBackground() {}
    bool DatabaseView::onEvent(SDL_Event* event) {
        return false;
    }

    void DatabaseView::onDraw() {
        ImGui::Text("%s", "Database");
    }
    
}