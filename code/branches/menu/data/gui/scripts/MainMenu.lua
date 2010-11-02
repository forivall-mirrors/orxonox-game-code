-- MainMenu.lua

local P = createMenuSheet("MainMenu")
P.loadAlong = { "SingleplayerMenu", "MultiplayerMenu", "SettingsMenu", "CreditsMenu" }

P.index = 0

-- events for MainMenu
function P.QuickGameTestButton_clicked(e)
    hideAllMenuSheets()
    orxonox.execute("startGame")
end

function P.onShow()

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

function P.onKeyPressed()
    --local we = tolua.cast(e, "CEGUI::KeyEventArgs")
    cout(0, code)
    if code == "15" then
        P.index = P.index + 1
        local window = winMgr:getWindow("orxonox/MainMenuBackground")
        if P.index == window:getChildCount() then
            P.index = 1
        end
        local child = window:getChildAtIdx(P.index-1)
        child:setProperty("NormalImageRightEdge", string.sub(child:getProperty("NormalImageRightEdge"),1,-7) .. "Highlight")
        child:setProperty("NormalImageLeftEdge", string.sub(child:getProperty("NormalImageLeftEdge"),1,-7) .. "Highlight")
        child:setProperty("NormalImageBackground", string.sub(child:getProperty("NormalImageBackground"),1,-7) .. "Highlight")
    elseif code == "28" and P.index ~= 0 then
        if P.index == 1 then
            P.QuickGameTestButton_clicked()
        elseif P.index == 2 then
            P.SingleplayerButton_clicked()
        elseif P.index == 3 then
            P.MultiplayerButton_clicked()
        elseif P.index == 4 then
            P.SettingsButton_clicked()
        elseif P.index == 5 then
            P.CreditsButton_clicked()
        elseif P.index == 6 then
            P.ExitButton_clicked()
        end
    end
end

return P
