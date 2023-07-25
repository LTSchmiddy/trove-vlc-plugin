io = require("io")
json = require("json")

-- Apparently, service directory scripts can't access VLC's config module,
-- so we're defining trove's config in the script directly. Besides,
-- The main thing vlc needs to know is where trove is located.

local trove_settings = {
    trove_cli_location = "..\\build\\x64-windows-Debug\\bin\\trove-cli.exe"
}

return trove_settings

