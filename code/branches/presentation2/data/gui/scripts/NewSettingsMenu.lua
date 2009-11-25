-- NewSettingsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewSettingsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewSettingsMenu"
P.layoutString = "NewSettingsMenu.layout"

function P.SettingsGameplayButton_clicked(e)
    showGUI("NewGameplayMenu")
    debug("event: GAMEPLAY")
end

function P.SettingsMultiplayerOptionsButton_clicked(e)
    showGUI("NewMultiplayerOptionsMenu")
    debug("event: MULTIPLAYER OPTIONS")
end

function P.SettingsControlsButton_clicked(e)
    showGUI("NewControlsMenu")
    debug("event: CONTROLS")
end

function P.SettingsGraphicsButton_clicked(e)
    showGUI("NewGraphicsMenu")
    debug("event: GRAPHICS")
end

function P.SettingsAudioButton_clicked(e)
    showGUI("NewAudioMenu")
    debug("event: AUDIO")
end

function P.SettingsResetSettingsButton_clicked(e)
    -- reset settings
    debug("event: reset settings")
end

function P.SettingsBackButton_clicked(e)
    hideGUI("NewSettingsMenu")
    debug("event: back")
end

return P

