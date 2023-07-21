#pragma once

#include "ns_abbr/fs.h"
#include "gfx_headers.h"
#include "ui/base/fragment.h"

#include "scripting/script_types/MovieScraperScript.h"
#include "scripting/script_types/MovieParserScript.h"

namespace UI
{
    class MainWindow : public Fragment {
    public:
        bool open = true;
        ImGuiWindowFlags flags =  0
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoMove
        ;
        ImVec2 margins = {0.0f, 0.0f};

        MainWindow();
        ~MainWindow();

        void onDraw() override;
    private:
        fs::path script_dir;
        Scripting::ScriptTypes::MovieScraperScript* scraper = NULL;
        Scripting::ScriptTypes::MovieParserScript* parser = NULL;

        std::string basicSearchParam;
        std::string scrapOutput;

        void drawParserTestInterface();
        void drawQueryInterface();

    };
} // namespace UI
