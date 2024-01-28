---@diagnostic disable: lowercase-global

-- Lazy Loading. Required implementation for Service Discovery:
lazily_loaded = false
-- Libraries:
io = nil
json = nil
trove_utils = nil

function lazy_load()
    if lazily_loaded then return nil end
    json = require("json")
    io = require("io")
    trove_utils = require("trove_utils")
    lazily_loaded = true
end

-- VLC Functions:
function descriptor()
    return {
        title = "Trove Media Library",
        version = "1.0",
        author = "",
        -- url = 'http://',
        url = 'file://',
        -- capabilities = {"search"}
        capabilities = {}
    }
end

function main()
    lazy_load()
    -- Load the data from disk:
    local f = io.popen(trove_utils:get_library_command())
    if f == nil then 
        return 
    end
    local json_str = f:read("*a")

    vlc.msg.dbg(json_str)
    library = json.decode(json_str)

    -- Process Movies:
    local movies_node = vlc.sd.add_node({title="Movies"})
    for index, movie in ipairs(library.movies) do
        -- local loc = vlc.strings.encode_uri_component(movie.location)
        local loc = vlc.strings.decode_uri(movie.location)
        movies_node:add_subitem({title = movie.title, description = movie.desc, date=movie.date, path=loc, arturl=vlc.strings.make_uri(movie.poster_path)})
    end

    -- Process TV Shows:
    local tv_shows_node = vlc.sd.add_node({title="TV Shows"})
    for show_index, show in ipairs(library.tv_shows) do
        local show_node = tv_shows_node:add_subnode({title = show.title, arturl=vlc.strings.make_uri(show.poster_path)})
        
        -- Process Seasons:
        for season_index, season in ipairs(show.seasons) do 
            local season_node = show_node:add_subnode({title = season.title, arturl=vlc.strings.make_uri(season.poster_path)})
            
            -- Process Episodes:
            for episode_index, episode in ipairs(season.episodes) do 
                local loc = vlc.strings.decode_uri(episode.location)
                season_node:add_subitem({
                    title = episode.title,
                    description = episode.desc,
                    date=episode.air_date,
                    path=loc,
                    showname=show.title,
                    season=episode.season,
                    episode=episode.episode,
                    arturl=vlc.strings.make_uri(episode.poster_path)
                })
            end
        end
    end

    -- Do stuff here
end

-- Internal Functions:

