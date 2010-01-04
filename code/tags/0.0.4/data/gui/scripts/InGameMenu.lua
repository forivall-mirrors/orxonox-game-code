-- InGameMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    InGameMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "InGameMenu"
P.layoutString = "InGameMenu.layout"

function P:init()
end


-- events for ingamemenu
function P.button_quit_clicked(e)
    openDecisionPopup( "Do you really want to quit the game?", InGameMenu.callback )
end

function P.button_mainmenu_clicked(e)
    orxonox.Game:getInstance():popState()
    orxonox.Game:getInstance():popState()
    orxonox.Game:getInstance():requestState("mainmenu")
    hideGUI("InGameMenu")
end

function P.button_settings_clicked(e)
    showGUI("SettingsMenu", true)
end

function P.button_return_clicked(e)
    hideGUI("InGameMenu")
end

function P.callback(doExit)
    if doExit then
        hideGUI("InGameMenu")
        orxonox.execute("exit")
    end
end

return P

