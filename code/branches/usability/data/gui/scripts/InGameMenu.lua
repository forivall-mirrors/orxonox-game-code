-- InGameMenu.lua

local P = createMenuSheet("InGameMenu")
P.loadAlong = { "DecisionPopup" }

function P.onLoad()
    P.multiplayerMode = "startClient"

    --button are arranged in a 4x1 matrix, the left lower item is nil
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_ReturnButton"),
            ["callback"]  = P.button_return_clicked
    })

    P:setButton(2, 1, {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_MainMenuButton"),
            ["callback"]  = P.button_mainmenu_clicked
    })

    P:setButton(3, 1, {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_SettingsButton"),
            ["callback"]  = P.button_settings_clicked
    })

    P:setButton(4, 1, {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_QuitButton"),
            ["callback"]  = P.button_quit_clicked
    })
end

function P.onShow()
    if P:hasSelection() == false then
        P:setSelection(1, 1)
    end

    orxonox.execute("setPause 1")
end

function P.onQuit()
    orxonox.execute("setPause 0")
end

-- events for ingamemenu
function P.button_quit_clicked(e)
    openDecisionPopup( "Do you really want to quit the game?", InGameMenu.callback )
end

function P.button_mainmenu_clicked(e)
    orxonox.execute("startMainMenu")
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
    else
        P.onShow()
    end
end

return P

