#pragma once
#include "gfx_headers.h"

#include <string>
#include <memory>

namespace UI {
    const ImGuiWindowFlags BG_WINDOW_FLAGS = 0
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoMove
        // | ImGuiWindowFlags_NoTitleBar
        ;

    class Fragment {
    public:
        // Return false if the event should be consumed:
        virtual bool onEvent(SDL_Event* event);
        
        virtual void onBackground();
        virtual void onDraw();
        virtual bool shouldDestroy();
    };

    class FWindow : public Fragment {
    public:
        std::string id;
        bool open = true;
        ImGuiWindowFlags flags = 0;
        std::shared_ptr<Fragment> content;

        FWindow(std::string p_id, std::shared_ptr<Fragment> p_content, bool p_start_open = true, ImGuiWindowFlags p_flags = 0);
        ~FWindow();

        bool onEvent(SDL_Event* event) override;
        void onBackground() override;
        void onDraw() override;
        bool shouldDestroy() override;
    };
}
