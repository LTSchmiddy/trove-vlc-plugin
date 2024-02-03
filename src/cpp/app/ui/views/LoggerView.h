#pragma once

#include <vector>
#include <memory>

#include "app/ui/base/fragment.h"
#include "core/library/library_globals.h"
#include "core/library/containers/MovieContainer.h"
#include "core/assets/AssetManager.h"
#include "core/assets/asset_wrappers/SDL_Texture_wrapper.h"

namespace UI::Views {
    class LoggerView : public Fragment {
    public:
        LoggerView();
        ~LoggerView();

        void onBackground() override;
        bool onEvent(SDL_Event* event) override;
        void onDraw() override;

    private:
        
    };
}