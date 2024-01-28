#pragma once

#include <string>
#include <memory>
#ifdef UI_BUILD
    #include "core/assets/asset_wrappers/SDL_Texture_wrapper.h"
#endif

#include "TvShowContainer.h"

namespace Library::Containers {
    class TvShowSearchTextContainer {
    public:

        // DB values:
        std::string search_text;
        std::string show_title;
        std::string show_date;
        std::string scraper_vars;
        
        TvShowSearchTextContainer();
        TvShowSearchTextContainer(std::string p_search_text, bool auto_load = true, bool* found = NULL);
        TvShowSearchTextContainer(std::string p_search_text, std::string p_show_title, std::string p_show_date, std::string p_scraper_info);
        ~TvShowSearchTextContainer();
        
        bool reloadFromDb();
        bool loadFromDb(std::string p_search_text);
        bool existsInDb();
        bool writeToDb();

        // Get referenced Tv Show:
        TvShowContainer getTvShow(bool* p_found = NULL);

    };
}