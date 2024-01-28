#pragma once

#include <string>
#include <memory>

#include "core/assets/asset_wrappers/SDL_Texture_wrapper.h"


#include "TvShowContainer.h"

namespace Library::Containers {
    class TvSeasonContainer {
    public:
        // DB values:
        std::string show_title;
        std::string show_date;
        int season = 0;
        std::string title;
        std::string desc;
        std::string poster_path;
        
        TvSeasonContainer();
        TvSeasonContainer(std::string p_show_title, std::string p_show_date, int p_season, bool auto_load = true, bool* found = NULL);
        TvSeasonContainer(std::string p_show_title, std::string p_show_date, int p_season, std::string p_title, std::string p_desc, std::string p_poster_path);
        ~TvSeasonContainer();
        
        bool reloadFromDb();
        bool loadFromDb(std::string p_show_title, std::string p_show_date, int p_season);
        bool existsInDb();
        bool writeToDb();

        // Get referenced Tv Show:
        TvShowContainer getTvShow(bool* p_found = NULL);


        // Poster Stuff
        std::shared_ptr<Assets::AssetWrappers::SdlTextureWrapper> poster_img = NULL;

        void loadPosterImg();


    };
}