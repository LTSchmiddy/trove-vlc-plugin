-- TMDB Movie Scraper
-- By LT_Schmiddy

string = require("string");

-- Constants
TMDB_API_KEY = "42cb439f9947906214fe04dc7ed1eb16"
TMDB_API_READ_TOKEN = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiI0MmNiNDM5Zjk5NDc5MDYyMTRmZTA0ZGM3ZWQxZWIxNiIsInN1YiI6IjVkNzI5Nzk4MjA5ZjE4NjkxNGZiMDhlZCIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.uyObV2lib7s23d7CoMKqHgzkrVAuzJauP4neA20AsLQ"
TMDB_REQUEST_HEADER = {"Authorization: Bearer " .. TMDB_API_READ_TOKEN}


-- Variables:

-- return title, date, description
function basic_episode_search(query_json, scraper_vars_json)
    print("Getting episode info...")
    -- print(query_json)
    -- print(scraper_vars_json)

    local query_dict = cjson.decode(query_json)
    local scraper_vars = cjson.decode(scraper_vars_json)

    local episode_info_json = web_requests.get(
        "https://api.themoviedb.org/3/tv/" .. scraper_vars.show_id .. "/season/" .. query_dict.season .. "/episode/" .. query_dict.episode, 
        TMDB_REQUEST_HEADER
    )

    local episode_info = cjson.decode(episode_info_json);
    
    -- Getting poster
    local poster_save_path = ""
    if episode_info.still_path ~= cjson.null and episode_info.still_path ~= nil then
        poster_save_path = config_access.get_asset_root() .. "/tmdb_episode_stills" .. episode_info.still_path
        -- log.debug(poster_save_path)
        web_requests.download_file("https://image.tmdb.org/t/p/original" .. episode_info.still_path, poster_save_path, TMDB_REQUEST_HEADER)
    end
    retVal = {
        episode_number = episode_info.season_number,
        season_number = episode_info.season_number,
        title = episode_info.name,
        air_date = episode_info.air_date,
        desc = episode_info.overview,
        poster_path = poster_save_path,
        _scraper_vars = {}
    }


    -- print(query_json)
    return cjson.encode(retVal)
end

function basic_season_search(query_json, scraper_vars_json)
    print("Getting season info...")
    local query_dict = cjson.decode(query_json)
    local scraper_vars = cjson.decode(scraper_vars_json)

    local season_info_json = web_requests.get(
        "https://api.themoviedb.org/3/tv/" .. scraper_vars.show_id .. "/season/" .. query_dict.season, 
        TMDB_REQUEST_HEADER
    )

    local season_info = cjson.decode(season_info_json);
    -- print(season_info_json)
    -- Getting poster
    local poster_save_path = config_access.get_asset_root() .. "/tmdb_seasons_posters" .. season_info.poster_path
    -- log.debug(season_info.poster_path)
    web_requests.download_file("https://image.tmdb.org/t/p/original" .. season_info.poster_path, poster_save_path, TMDB_REQUEST_HEADER)

    retVal = {
        season_number = season_info.season_number,
        title = season_info.name,
        desc = season_info.overview,
        poster_path = poster_save_path,
        _scraper_vars = {}
    }

    return cjson.encode(retVal)    
end

function basic_show_search(query_json)
    print("Getting show info...")
    local query_dict = cjson.decode(query_json)

    local r = cjson.decode(web_requests.get_args(
        "https://api.themoviedb.org/3/search/tv", 
        {
            query = query_dict["show_name"], 
            year = query_dict["show_year"]
        }, 
        TMDB_REQUEST_HEADER
    ))

    if r.total_results == 0 then return "{}" end

    -- We're going to use the first show in the search results:
    local selected_show_id = "" .. r.results[1].id

    local show_info_json = web_requests.get(
        "https://api.themoviedb.org/3/tv/" .. selected_show_id, 
        TMDB_REQUEST_HEADER
    )

    local show_info = cjson.decode(show_info_json);
    
    -- Getting poster
    local poster_save_path = config_access.get_asset_root() .. "/tmdb_shows_posters" .. show_info.poster_path
    -- log.debug(poster_save_path)
    web_requests.download_file("https://image.tmdb.org/t/p/original" .. show_info.poster_path, poster_save_path, TMDB_REQUEST_HEADER)

    retVal = {
        title = show_info.original_name,
        date = show_info.first_air_date,
        seasons = show_info.number_of_seasons,
        desc = show_info.overview,
        poster_path = poster_save_path,
        _scraper_vars = {
            show_id = selected_show_id,
        }
    }

    -- print(cjson.encode(retVal))
    return cjson.encode(retVal)
end
