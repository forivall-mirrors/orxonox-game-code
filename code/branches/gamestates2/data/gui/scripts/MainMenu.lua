-- MainMenu.lua

local P = createMenuSheet("MainMenu")

-- events for MainMenu
function P.QuickGameTestButton_clicked(e)
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

