-- NewMainMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewMainMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewMainMenu"
P.layoutString = "NewMainMenu.layout"

-- events for NewMainMenu
function P.QuickGameTestButton_clicked(e)
    orxonox.CommandExecutor:execute("startGame")
    debug("event: quick game test")
end

function P.SingleplayerButton_clicked(e)
    showGUI("NewSingleplayerMenu")
    debug("event: SINGLEPLAYER")
end

function P.MultiplayerButton_clicked(e)
    showGUI("NewMultiplayerMenu")
    debug("event: MULTIPLAYER")
end

function P.SettingsButton_clicked(e)
    showGUI("NewSettingsMenu")
    debug("event: SETTINGS")
end

function P.CreditsButton_clicked(e)
    showGUI("NewCreditsMenu")
    debug("event: CREDITS")
end

function P.ExitButton_clicked(e)
    orxonox.CommandExecutor:execute("exit")
    debug("event: exit")
end

return P

