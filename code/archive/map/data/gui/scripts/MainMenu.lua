-- MainMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    MainMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "MainMenu"
P.layoutString = "MainMenu.layout"

-- events for MainMenu
function P.QuickGameTestButton_clicked(e)
    orxonox.execute("startGame")
end

function P.SingleplayerButton_clicked(e)
    showGUI("SingleplayerMenu", true)
end

function P.MultiplayerButton_clicked(e)
    showGUI("MultiplayerMenu", true)
end

function P.SettingsButton_clicked(e)
    showGUI("SettingsMenu", true)
end

function P.CreditsButton_clicked(e)
    showGUI("CreditsMenu", true)
end

function P.ExitButton_clicked(e)
    orxonox.execute("exit")
end

return P

