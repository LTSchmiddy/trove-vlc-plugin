-- TMDB Movie Scraper

string = require("string")

function string_trim(s)
    return string.gsub(s,"^%s*(.-)%s*$", "%1")
end

log.debug("Hello")

function year_parse_method(str, retVal)
    local y_match = {}
    if regex.search(str, "(\\(\\d\\d\\d\\d\\))", y_match, {}, {}) then
        -- print(cjson.encode(y_match))
        local year_string = string.sub(y_match.index[1].str, 2, y_match.index[1].length - 1)
        retVal["show_year"] = year_string
        retVal["show_name"] = string_trim(y_match.prefix.str)
        retVal["_tv_show_search_text"] = string_trim(y_match.prefix.str)
        return true
    else 
        return false
    end
end

function get_season_episode(str, retVal)
    local y_match = {}
    if regex.search(str, "(S\\d+E\\d+)", y_match, {"icase"}, {}) then
        -- print(cjson.encode(y_match))
        -- Get the actual numbers:
        -- Season
        local season_info = {}
        regex.search(y_match.index[1].str, "(S\\d+)", season_info, {"icase"}, {})
        local season_str = season_info.index[1].str
        retVal["season"] = tonumber(string.sub(season_str, 2, string.len(season_str)))
        
        -- Episode
        local episode_info = {}
        regex.search(y_match.index[1].str, "(E\\d+)", episode_info, {"icase"}, {})
        local episode_str = episode_info.index[1].str
        retVal["episode"] = tonumber(string.sub(episode_str, 2, string.len(episode_str)))

        return true
    else 
        return false
    end
end

-- REQUIRED FUNCTIONS
-- Return search parameters based on filepath. Paremeters should be in json format.
function parse_path(dir, filename)
    
    local parse_info = {}
    local retVal = {}
    if year_parse_method(dir, parse_info) and get_season_episode(filename, parse_info) then

        retVal = {
            show = {
                show_name = parse_info["show_name"],
                show_year = parse_info["show_year"],
                _tv_show_search_text = parse_info["show_name"]
            },
            season = {
                show_name = parse_info["show_name"],
                show_year = parse_info["show_year"],
                season = parse_info["season"],
                _tv_show_season_number = parse_info["season"]
            },
            episode = {
                show_name = parse_info["show_name"],
                show_year = parse_info["show_year"],
                season = parse_info["season"],
                episode = parse_info["episode"],
            }
        }
    else 
        retVal = {}
    end

    local s_retVal = cjson.encode(retVal)
    -- print(s_retVal)
    return s_retVal
    
end