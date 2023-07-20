#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>

#include "gfx_headers.h"
#include "ui/base/fragment.h"

namespace UI {
    class SubWindowHandler {
    public:
        SubWindowHandler();
        ~SubWindowHandler();

        void handleNewWindows();
        void handleBackgrounds();
        void handleEvents(SDL_Event* event);
        void handleDraw();
        void handleDeletion();

        // Returns false on failure:
        bool addSubWindow(std::string id, std::shared_ptr<UI::Fragment> new_window);
        // Returns false on failure:
        bool markForDeletion(std::string id);
        bool exists(std::string id);
        
        std::shared_ptr<UI::Fragment> get(std::string id);

    private:
        std::map<std::string, std::shared_ptr<UI::Fragment>> new_sub_windows;
        std::map<std::string, std::shared_ptr<UI::Fragment>> sub_windows;
        std::vector<std::string> sub_window_deletion_list;

    };
}