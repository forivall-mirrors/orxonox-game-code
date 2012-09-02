-- MainMenu.lua

local P = createMenuSheet("MainMenu")
P.loadAlong = { "SingleplayerMenu", "MultiplayerMenu", "SettingsMenu", "CreditsMenu" }

function P.onLoad()
    --buttons are arranged in a 6x1 Matrix (list)
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/QuickGameTestButton"),
            ["callback"]  = P.QuickGameTestButton_clicked
    })

    P:setButton(2, 1, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerButton"),
            ["callback"]  = P.SingleplayerButton_clicked
    })

    P:setButton(3, 1, {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerButton"),
            ["callback"]  = P.MultiplayerButton_clicked
    })

    P:setButton(4, 1, {
            ["button"] = winMgr:getWindow("orxonox/SettingsButton"),
            ["callback"]  = P.SettingsButton_clicked
    })

    P:setButton(5, 1, {
            ["button"] = winMgr:getWindow("orxonox/CreditsButton"),
            ["callback"]  = P.CreditsButton_clicked
    })

    P:setButton(6, 1, {
            ["button"] = winMgr:getWindow("orxonox/ExitButton"),
            ["callback"]  = P.ExitButton_clicked
    })
end

-- events for MainMenu
function P.QuickGameTestButton_clicked(e)
    hideAllMenuSheets()
    orxonox.execute("startGame")
end

function P.SingleplayerButton_clicked(e)
    showMenuSheet("SingleplayerMenu", true)
end

function P.MultiplayerButton_clicked(e)
    showMenuSheet("MultiplayerMenu", true)
end

function P.SettingsButton_clicked(e)
    showMenuSheet("SettingsMenu", true)
end

function P.CreditsButton_clicked(e)
    showMenuSheet("CreditsMenu", true)
end

function P.ExitButton_clicked(e)
    orxonox.execute("exit")
end

return P

