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
    virtual bool on_event(SDL_Event* event);
    virtual void on_background();
    virtual void on_draw();
};

class FWindow : public Fragment {
public:
    std::string id;
    bool open = true;
    ImGuiWindowFlags flags = 0;
    Fragment* content;

    bool destruct_content = false;

    FWindow(std::string p_id, Fragment* p_content, bool p_destruct_content = false, bool p_start_open = true, ImGuiWindowFlags p_flags = 0);
    ~FWindow();

    bool on_event(SDL_Event* event) override;
    void on_background() override;
    void on_draw() override;
};
}
