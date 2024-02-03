#include <sqlite3.h>

#include "LoggerView.h"

namespace UI::Views {
    LoggerView::LoggerView(){
        
    }
    LoggerView::~LoggerView(){}

    void LoggerView::onBackground() {}
    bool LoggerView::onEvent(SDL_Event* event) {
        return false;
    }

    void LoggerView::onDraw() {
        ImGui::Text("%s", "Logger");
    }
    
}