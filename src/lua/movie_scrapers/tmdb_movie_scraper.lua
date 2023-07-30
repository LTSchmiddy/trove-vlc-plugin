-- TMDB Movie Scraper
-- By LT_Schmiddy

string = require("string");

TMDB_API_KEY = "42cb439f9947906214fe04dc7ed1eb16"
TMDB_API_READ_TOKEN = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiI0MmNiNDM5Zjk5NDc5MDYyMTRmZTA0ZGM3ZWQxZWIxNiIsInN1YiI6IjVkNzI5Nzk4MjA5ZjE4NjkxNGZiMDhlZCIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.uyObV2lib7s23d7CoMKqHgzkrVAuzJauP4neA20AsLQ"
TMBD_REQUEST_HEADER = {"Authorization: Bearer " .. TMDB_API_READ_TOKEN}

-- return title, date, description
function basic_search(query_json)
    query_dict = cjson.decode(query_json)

    local search_result = web_requests.get_args(
        "https://api.themoviedb.org/3/search/movie", 
        {
            query = query_dict["name"], 
            year = query_dict["year"]
        }, 
        TMBD_REQUEST_HEADER
    )
    local r = cjson.decode(search_result)

    -- if no results are found, return nothing:
    if r.total_results == 0 then return "{}" end

    -- We're going to use the first movie in the search results:
    selected_movie_id = "" .. r.results[1].id

    local movie_info_json = web_requests.get(
        "https://api.themoviedb.org/3/movie/" .. selected_movie_id, 
        TMBD_REQUEST_HEADER
    )
    
    -- Since we pulled the id straight from another response, we can assume it's valid.
    local movie_info = cjson.decode(movie_info_json)

    local poster_save_path = config_access.get_asset_root() .. "/tmdb_movies_posters" .. movie_info.poster_path
    -- log.debug(poster_save_path)
    web_requests.download_file("https://image.tmdb.org/t/p/original" .. movie_info.poster_path, poster_save_path, TMBD_REQUEST_HEADER)

    return cjson.encode ({
        title = movie_info.original_title,
        date = movie_info.release_date,
        desc = movie_info.overview,
        id = movie_info.id,
        poster_path = poster_save_path
    }) 
end
