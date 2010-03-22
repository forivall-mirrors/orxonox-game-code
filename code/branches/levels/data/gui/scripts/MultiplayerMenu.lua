-- MultiplayerMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    MultiplayerMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "MultiplayerMenu"
P.layoutString = "MultiplayerMenu.layout"

function P:init()
    listbox = winMgr:getWindow("orxonox/MultiplayerLevelListbox")
    preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    orxonox.LevelManager:getInstance():compileAvailableLevelList()
    local levelList = {}
    local index = 0
    local level = ""
    while true do
        level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
        if level == "" then
            break
        end
        table.insert(levelList, level)
        index = index + 1
    end
    table.sort(levelList)
    for k,v in pairs(levelList) do
        item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end
    local multiplayerMode = "startClient"
    if multiplayerMode == "startClient" then
        window = winMgr:getWindow("orxonox/MultiplayerJoinButton")
        button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
    if multiplayerMode == "startServer" then
        window = winMgr:getWindow("orxonox/MultiplayerHostButton")
        button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
    if multiplayerMode == "startDedicated" then
        window = winMgr:getWindow("orxonox/MultiplayerDedicatedButton")
        button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
end

function P.MultiplayerJoinButton_clicked(e)
    multiplayerMode = "startClient"
end

function P.MultiplayerHostButton_clicked(e)
    multiplayerMode = "startServer"
end

function P.MultiplayerDedicatedButton_clicked(e)
    multiplayerMode = "startDedicated"
end

function P.MultiplayerStartButton_clicked(e)
    local choice = winMgr:getWindow("orxonox/MultiplayerLevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.execute(multiplayerMode)
        hideAllGUIs()
    end
end

function P.MultiplayerBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

