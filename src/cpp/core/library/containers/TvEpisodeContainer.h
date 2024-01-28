#pragma once

#include <string>
#include <memory>

#include "core/assets/asset_wrappers/SDL_Texture_wrapper.h"


#include "TvShowContainer.h"
#include "TvSeasonContainer.h"

namespace Library::Containers {
    class TvEpisodeContainer {
    public:
        // DB values:
        std::string source;
        std::string path;
        std::string title;
        std::string show_title;
        std::string show_date;
        int season = 0;
        int episode = 0;
        std::string air_date;
        std::string desc;
        std::string poster_path;
        
        TvEpisodeContainer();
        TvEpisodeContainer(std::string p_source, std::string p_path, bool auto_load = true, bool* found = NULL);
        TvEpisodeContainer(
            std::string p_source,
            std::string p_path,
            std::string p_title,
            std::string p_show_title,
            std::string p_show_date,
            int p_season,
            int p_episode,
            std::string p_air_date,
            std::string p_desc,
            std::string p_poster_path
        );
        ~TvEpisodeContainer();

        bool reloadFromDb();
        bool loadFromDb(std::string p_source, std::string p_path);
        bool existsInDb();
        bool writeToDb();

        // Get referenced Tv Show:
        TvShowContainer getTvShow(bool* p_found = NULL);

        // Get referenced Tv Show:
        TvSeasonContainer getTvSeason(bool* p_found = NULL);


        // Poster Stuff
        std::shared_ptr<Assets::AssetWrappers::SdlTextureWrapper> poster_img = NULL;

        void loadPosterImg();
    };
}