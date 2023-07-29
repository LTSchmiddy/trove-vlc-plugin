-- TMDB Movie Scraper
-- By LT_Schmiddy

string = require("string");

-- Constants
TMDB_API_KEY = "42cb439f9947906214fe04dc7ed1eb16"
TMDB_API_READ_TOKEN = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiI0MmNiNDM5Zjk5NDc5MDYyMTRmZTA0ZGM3ZWQxZWIxNiIsInN1YiI6IjVkNzI5Nzk4MjA5ZjE4NjkxNGZiMDhlZCIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.uyObV2lib7s23d7CoMKqHgzkrVAuzJauP4neA20AsLQ"
TMBD_REQUEST_HEADER = {"Authorization: Bearer " .. TMDB_API_READ_TOKEN}
basic_episode_required_values = {
    "show_name",
    "show_year",
    "season",
    "episode",
}

-- Variables
looked_up_shows = {}

-- return title, date, description
function basic_episode_search(query_json)
    query_dict = cjson.decode(query_json)

    -- Verify that we have all the lookup infor we need:
    for _, val in ipairs(basic_episode_required_values) do
        if query_dict[val] == nil then
            log.error("Function basic_episode_search did not recieve parameter " .. val)
            return cjson.encode({})
        end
    end

    local search_result = cjson.decode(web_requests.get_args(
        "https://api.themoviedb.org/3/search/tv", 
        {
            query = query_dict["name"], 
            year = query_dict["year"]
        }, 
        TMBD_REQUEST_HEADER
    ))


    print(query_json)
    return cjson.encode({})
end

function basic_season_search(query_json)

end

function basic_show_search(query_json)

end
