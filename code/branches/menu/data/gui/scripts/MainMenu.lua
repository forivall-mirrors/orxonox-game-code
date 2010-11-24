-- MainMenu.lua

local P = createMenuSheet("MainMenu")
P.loadAlong = { "SingleplayerMenu", "MultiplayerMenu", "SettingsMenu", "CreditsMenu" }

P.buttonList = {}

P.testArray = {}


function P.onLoad()
    local item = {
            ["button"] = winMgr:getWindow("orxonox/QuickGameTestButton"),
            ["function"]  = P.QuickGameTestButton_clicked
    }
    table.insert(P.buttonList,item)

    item = {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerButton"),
            ["function"]  = P.SingleplayerButton_clicked
    }
    table.insert(P.buttonList,item)

    item = {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerButton"),
            ["function"]  = P.MultiplayerButton_clicked
    }
    table.insert(P.buttonList,item)

    item = {
            ["button"] = winMgr:getWindow("orxonox/SettingsButton"),
            ["function"]  = P.SettingsButton_clicked
    }
    table.insert(P.buttonList,item)

    item = {
            ["button"] = winMgr:getWindow("orxonox/CreditsButton"),
            ["function"]  = P.CreditsButton_clicked
    }
    table.insert(P.buttonList,item)

    item = {
            ["button"] = winMgr:getWindow("orxonox/ExitButton"),
            ["function"]  = P.ExitButton_clicked
    }
    table.insert(P.buttonList,item) 
end

function P.onShow()
    P.oldindex = -2
    P.index = -1
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


--[[
list = {}
local item =
{
    ["button"] = buttonWindow,
    ["function"]  = buttonFunction,
}
table.insert(list, item)
item = list[i]

for i,item in pairs(list) do
    button = item["button"]
end

--]]
--[[
function createList()
    list = {}

    local j 
    while j < P.loadAlong 
        local item =
        {
            ["button"] = buttonWindow,
            ["function"]  = buttonFunction,
        }
        table.insert(list, item)
    end

--]]

function P.onKeyPressed() 
    cout(0,code)
    buttonIteratorHelper(P.buttonList, code, P, 6, 1)
    --indexTester(P.buttonList,code,P,6,1)
end

return P

