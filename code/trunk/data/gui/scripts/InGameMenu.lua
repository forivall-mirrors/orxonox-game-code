-- InGameMenu.lua

local P = createMenuSheet("InGameMenu")
P.loadAlong = { "DecisionPopup" }

P.buttonList = {}

function P.onLoad()
    P.multiplayerMode = "startClient" 

    --button are arranged in a 4x1 matrix, the left lower item is nil
    local item = {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_ReturnButton"),
            ["function"]  = P.button_settings_clicked
    }
    P.buttonList[1] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_MainMenuButton"),
            ["function"]  = P.button_mainmenu_clicked
    }
    P.buttonList[2] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_SettingsButton"),
            ["function"]  = P.button_settings_clicked
    }
    P.buttonList[3] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/InGameMenu_QuitButton"),
            ["function"]  = P.button_quit_clicked
    }
    P.buttonList[4] = item

end

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
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

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 4, 1)
end

return P

