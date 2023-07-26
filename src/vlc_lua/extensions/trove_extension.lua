---@diagnostic disable: lowercase-global
-- "extension.lua"
-- VLC Extension basic structure (template): ----------------

-- Install
-- Windows: %APPDATA%/vlc/lua/extensions/basic.lua
-- Mac:     /Applications/VLC/.../lua/extensions/basic.lua
-- Linux:   ~/.local/share/vlc/lua/extensions/basic.lua

os = require("os")
io = require("io")
package = require("package")

json = require("json")
trove_utils = require("trove_utils")

function descriptor()
    return {
        title = "VLC Extension - Basic structure",
        version = "1.0",
        author = "",
        url = 'http://',
        shortdesc = " Open Trove Manager",
        description = "full description",
        capabilities = {"trigger", "input-listener", "meta-listener", "playing-listener"}
    }
end

function trigger()
    -- this is where extension starts

    vlc.msg.dbg("Opening Trove Manager...")
    vlc.msg.dbg(trove_utils:start_manager_command())
    -- os.execute(trove_utils:get_manager_path())
    io.popen(trove_utils:start_manager_command())
end

function input_changed()
    -- related to capabilities={"input-listener"} in descriptor()
    -- triggered by Start/Stop media input event
end

function playing_changed()
    -- related to capabilities={"playing-listener"} in descriptor()
    -- triggered by Pause/Play madia input event
end

function meta_changed()
    -- related to capabilities={"meta-listener"} in descriptor()
    -- triggered by available media input meta data?
end
