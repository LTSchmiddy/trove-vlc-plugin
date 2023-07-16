---@diagnostic disable: lowercase-global

-- Lazy Loading. Required implementation for Service Discovery
lazily_loaded = false
io = nil
json = nil

function lazy_load()
    if lazily_loaded then return nil end
    json = require("json")
    io = require("io")
    lazily_loaded = true
end

function read_data(uri)
    local stream = vlc.stream(uri)
    local string = ""
    local line   = ""

    repeat
      line = stream:readline()
      if line then string = string .. line end
    until line == nil

    return string
end

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

    local f = io.open("hello.txt")
    vlc.msg.dbg(f:read("*a"))

    local movie_node = vlc.sd.add_node({title="Movies", arturl=vlc.strings.make_uri("art.jpg")})
    local show_node = vlc.sd.add_node({title="TV Shows"})

    -- Do stuff here
end
  
  -- function search(query)
    
    -- Do stuff here
  -- end