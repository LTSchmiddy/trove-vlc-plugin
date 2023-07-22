-- path = "House.S01E01.720p.BluRay.x264.300MB-Pahe.in.mkv"
-- expr = "(S\\d+E\\d+)"
-- regex_out = {}
-- regex.search_iter(path, expr, regex_out, {}, {})
-- print(cjson.encode(regex_out))


function parse_path(dir, filename)
    -- Get the movie year:
    local y_match = {}
    regex.search(dir, "(\\(\\d\\d\\d\\d\\))", y_match, {}, {})
    print(cjson.encode(y_match))

    return "{}"
    
end