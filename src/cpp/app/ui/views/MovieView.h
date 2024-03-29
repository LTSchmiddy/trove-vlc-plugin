#pragma once

#include <vector>
#include <memory>

#include "app/ui/base/fragment.h"
#include "core/library/library_globals.h"
#include "core/library/containers/MovieContainer.h"
#include "core/assets/AssetManager.h"
#include "core/assets/asset_wrappers/SDL_Texture_wrapper.h"

namespace UI::Views {
    class MovieView : public Fragment {
    public:
        MovieView();
        ~MovieView();

        enum class LAYOUT {
            LIST = 0,
            GRID = 1
        } layout = LAYOUT::LIST;

        void reloadMovieList();

        void onBackground() override;
        bool onEvent(SDL_Event* event) override;
        void onDraw() override;

    private:
        float entry_size = 300.0f;
        std::vector<Library::Containers::MovieContainer> movie_list;
        
        void drawListLayout();
    };
}