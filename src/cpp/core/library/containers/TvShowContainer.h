#pragma once

#include <string>
#include <memory>
#ifdef UI_BUILD
    #include "core/assets/asset_wrappers/SDL_Texture_wrapper.h"
#endif

namespace Library::Containers {
    class TvShowContainer {
    public:
        // DB values:
        std::string title;
        std::string date;
        int seasons = 0;
        std::string desc;
        std::string poster_path;
        
        TvShowContainer();
        TvShowContainer(std::string p_title, std::string p_date, bool auto_load = true, bool* found = NULL);
        TvShowContainer(std::string p_title, std::string p_date, int p_seasons, std::string p_desc, std::string p_poster_path);
        ~TvShowContainer();

        bool reloadFromDb();
        bool loadFromDb(std::string p_title, std::string p_date);
        bool existsInDb();
        bool writeToDb();

#ifdef UI_BUILD
        // Poster Stuff
        std::shared_ptr<Assets::AssetWrappers::SdlTextureWrapper> poster_img = NULL;

        void loadPosterImg();
#endif

    };
}