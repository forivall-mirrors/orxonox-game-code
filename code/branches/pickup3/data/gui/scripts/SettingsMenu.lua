-- SettingsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    SettingsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "SettingsMenu"
P.layoutString = "SettingsMenu.layout"

function P.SettingsGameplayButton_clicked(e)
    showGUI("GameplayMenu", true)
end

function P.SettingsMultiplayerOptionsButton_clicked(e)
    showGUI("MultiplayerOptionsMenu", true)
end

function P.SettingsControlsButton_clicked(e)
    showGUI("ControlsMenu", true)
end

function P.SettingsGraphicsButton_clicked(e)
    showGUI("GraphicsMenu", true)
end

function P.SettingsAudioButton_clicked(e)
    showGUI("AudioMenu", true)
end

function P.SettingsResetSettingsButton_clicked(e)
    -- reset settings
    debug("event: reset settings")
end

function P.SettingsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

