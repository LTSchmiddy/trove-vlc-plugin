#pragma once

#include <string>
#include <memory>
#ifdef UI_BUILD
    #include "assets/asset_wrappers/SDL_Texture_wrapper.h"
#endif

#include "TvShowContainer.h"

namespace Library::Containers {
    class TvShowSearchTextContainer {
    public:

        // DB values:
        std::string search_text;
        std::string show_title;
        std::string show_date;
        
        TvShowSearchTextContainer();
        TvShowSearchTextContainer(std::string p_search_text, int p_season, bool auto_load = true, bool* found = NULL);
        TvShowSearchTextContainer(std::string p_search_text, std::string p_show_title, std::string p_show_date);
        ~TvShowSearchTextContainer();
        
        bool reloadFromDb();
        bool loadFromDb(std::string p_search_text);
        bool existsInDb();
        bool writeToDb();

        // Get referenced Tv Show:
        TvShowContainer getTvShow(bool* p_found = NULL);

#ifdef UI_BUILD
        // Poster Stuff
        std::shared_ptr<Assets::AssetWrappers::SdlTextureWrapper> poster_img = NULL;

        void loadPosterImg();
#endif

    };
}