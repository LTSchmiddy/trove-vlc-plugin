#pragma once
#include "gfx_headers.h"

#include <string>

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
    virtual bool onEvent(SDL_Event* event);
    virtual void onBackground();
    virtual void onDraw();
};

class FWindow : public Fragment {
public:
    std::string id;
    bool open = true;
    ImGuiWindowFlags flags = 0;
    Fragment* content;

    bool destructContent = false;

    FWindow(std::string p_id, Fragment* p_content, bool p_destruct_content = false, bool p_start_open = true, ImGuiWindowFlags p_flags = 0);
    ~FWindow();

    bool onEvent(SDL_Event* event) override;
    void onBackground() override;
    void onDraw() override;
};
}
