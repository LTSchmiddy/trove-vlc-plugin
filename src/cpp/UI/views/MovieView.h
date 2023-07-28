#pragma once

#include <vector>
#include <memory>

#include "ui/base/fragment.h"
#include "library/library_globals.h"
#include "library/containers/MovieContainer.h"
#include "assets/AssetManager.h"
#include "assets/asset_wrappers/SDL_Texture_wrapper.h"

namespace UI::Views {
    class MovieView : public Fragment {
    public:
        MovieView();
        ~MovieView();

        void reloadMovieList();

        void onBackground() override;
        bool onEvent(SDL_Event* event) override;
        void onDraw() override;

    private:
        std::vector<Library::Containers::MovieContainer> movie_list;   
    };
}