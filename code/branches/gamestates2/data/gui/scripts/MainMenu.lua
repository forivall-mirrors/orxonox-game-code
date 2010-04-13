-- MainMenu.lua

local P = createMenuSheet("MainMenu")

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

