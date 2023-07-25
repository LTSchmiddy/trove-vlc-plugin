---@diagnostic disable: lowercase-global

-- Lazy Loading. Required implementation for Service Discovery:
lazily_loaded = false
-- Libraries:
io = nil
json = nil
trove_settings = nil

function lazy_load()
    if lazily_loaded then return nil end
    json = require("json")
    io = require("io")
    trove_settings = require("trove_settings")
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
    local f = io.popen(trove_dump_cmd())
    if f == nil then 
        return 
    end
    local json_str = f:read("*a")

    vlc.msg.dbg(json_str)
    library = json.decode(json_str)

    -- Process Movies:
    local movie_node = vlc.sd.add_node({title="Movies"})
    for index, movie in ipairs(library.movies) do
        -- local loc = vlc.strings.encode_uri_component(movie.location)
        local loc = vlc.strings.decode_uri(movie.location)
        vlc.msg.dbg(loc)
        movie_node:add_subitem({title = movie.title, description = movie.desc, date=movie.date, path=loc, arturl=vlc.strings.make_uri(movie.poster_path)})
    end

    local show_node = vlc.sd.add_node({title="TV Shows"})

    -- Do stuff here
end

-- Internal Functions:

function trove_dump_cmd()
    return "\"" .. trove_settings.trove_cli_location .. "\" -d"
end
