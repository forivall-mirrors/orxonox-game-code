-- InGameMenu.lua

local P = createMenuSheet("InGameMenu")
P.loadAlong = { "DecisionPopup" }

-- events for ingamemenu
function P.button_quit_clicked(e)
    openDecisionPopup( "Do you really want to quit the game?", InGameMenu.callback )
end

function P.button_mainmenu_clicked(e)
    orxonox.Game:getInstance():popState()
    orxonox.Game:getInstance():popState()
    orxonox.Game:getInstance():requestState("mainmenu")
    hideMenuSheet("InGameMenu")
end

function P.button_settings_clicked(e)
    showMenuSheet("SettingsMenu", true)
end

function P.button_return_clicked(e)
    hideMenuSheet("InGameMenu")
end

function P.callback(doExit)
    if doExit then
        hideMenuSheet("InGameMenu")
        orxonox.execute("exit")
    end
end

return P

