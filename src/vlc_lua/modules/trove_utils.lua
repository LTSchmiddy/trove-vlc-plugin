io = require("io")
package = require("package")
trove_settings = require("trove_settings")

trove_utils = {

    get_path_seperator = function (self)
        return package.config:sub(1,1)
    end,

    is_unix = function (self)
        return self:get_path_seperator() ~= "\\"
    end,

    get_manager_path = function (self)
        local retVal = trove_settings.trove_location .. self:get_path_seperator() .. "trove-manager"
        if not self:is_unix() then
            retVal = retVal .. ".exe"
        end
        return retVal
    end,

    get_cli_path = function (self)
        local retVal = trove_settings.trove_location .. self:get_path_seperator() .. "trove-cli"
        if not self:is_unix() then
            retVal = retVal .. ".exe"
        end
        return retVal
    end,

    get_library_command = function (self)
        return "\"" .. self:get_cli_path() .. "\" -d"
    end


}

return trove_utils