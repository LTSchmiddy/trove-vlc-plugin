---@diagnostic disable: lowercase-global

-- Lazy Loading. Required implementation for Service Discovery
lazily_loaded = false
json = nil

function lazy_load()
    if lazily_loaded then return nil end
    json = require("json")
    lazily_loaded = true
end

function descriptor()
    return {
        title = "Trove Media Library",
        version = "1.0",
        author = "",
        url = 'http://',
        -- capabilities = {"search"}
        capabilities = {}
    }
end

function main()
    lazy_load()

    local movie_node = vlc.sd.add_node({title="Movies", arturl=vlc.strings.make_uri("art.jpg")})
    local show_node = vlc.sd.add_node({title="TV Shows"})

    -- Do stuff here
end
  
  -- function search(query)
    
    -- Do stuff here
  -- end