string = require("string")

function string_trim(s)
    return string.gsub(s,"^%s*(.-)%s*$", "%1")
end


-- REQUIRED FUNCTIONS
function year_parse_method(str, retVal)
    local y_match = {}
    if regex.search(str, "(\\(\\d\\d\\d\\d\\))", y_match, {}, {}) then
        -- print(cjson.encode(y_match))
        local year_string = string.sub(y_match.index[1].str, 2, y_match.index[1].length - 1)
        retVal["year"] = year_string
        retVal["name"] = string_trim(y_match.prefix.str)
        return true
    else 
        return false
    end
end

-- Return search parameters based on filepath. Paremeters should be in json format.
function parse_path(dir, filename)
    local retVal = {}
    
    --Prioritize directory name first
    if year_parse_method(dir, retVal) then
        -- cjson.encode(retVal)
    elseif year_parse_method(filename, retVal) then
        -- cjson.encode(retVal)
    else
        log.info("Could not parse movie path for " .. dir .. "/" .. filename)
    end
    local s_retVal = cjson.encode(retVal)
    -- print(s_retVal)
    return s_retVal
    
end