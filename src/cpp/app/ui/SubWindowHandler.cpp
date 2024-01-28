#include <plog/Log.h>

#include "SubWindowHandler.h"

namespace UI {
    SubWindowHandler::SubWindowHandler() {}
    SubWindowHandler::~SubWindowHandler() {}

    void SubWindowHandler::handleNewWindows() {
        for ( const auto &it : new_sub_windows ) {
            sub_windows.emplace(it.first, it.second);
        } 
        new_sub_windows.clear();
    }


    void SubWindowHandler::handleBackgrounds() {
        for ( const auto &it : sub_windows ) {
            it.second->onBackground();
        } 
    }

    void SubWindowHandler::handleEvents(SDL_Event* event) {
        for ( const auto &it : sub_windows ) {
            // If a subwindow consumes the event, stop passing the event to more windows:
            if (it.second->onEvent(event)) {
                break;
            }
        }
    }

    void SubWindowHandler::handleDraw() {
        for ( const auto &it : sub_windows ) {
            it.second->onDraw();
        } 
    }

    void SubWindowHandler::handleDeletion() {
        // Lets see if any sub_windows want to be to be deleted:
        for ( const auto &it : sub_windows ) {
            // std::cout << p.first << '\t' << p.second << std::endl;
            if (it.second->shouldDestroy()) {
                sub_window_deletion_list.push_back(it.first);
            }
        }
        // Delete any sub_windows that need it:
        for (int i = 0; i < sub_window_deletion_list.size(); i++) {
            // We'll check if the key actually exists, in case one is added twice (maybe one was marked for deletion elsewhere).
            if (sub_windows.contains(sub_window_deletion_list[i])) {
                sub_windows.erase(sub_window_deletion_list[i]);
            }
        }

        sub_window_deletion_list.clear();
    }

    bool SubWindowHandler::addSubWindow(std::string id, std::shared_ptr<UI::Fragment> new_window) {
        if (sub_windows.contains(id)) {
            PLOGE.printf("Subwindow with id '%s' already exists.", id.c_str());
            return false;
        }

        new_sub_windows.emplace(id, new_window);
        return false;
    }

    bool SubWindowHandler::markForDeletion(std::string id) {
        if (!sub_windows.contains(id)) {
            PLOGE.printf("No subwindow with id '%s' exists.", id.c_str());
            return false;
        }

        sub_window_deletion_list.push_back(id);
        return false;
    }

    bool SubWindowHandler::exists(std::string id) {
        return sub_windows.contains(id);
    }

    std::shared_ptr<UI::Fragment> SubWindowHandler::get(std::string id) {
        return sub_windows.at(id);
    }
}