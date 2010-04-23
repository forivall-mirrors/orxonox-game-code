-- MultiplayerMenu.lua

local P = createMenuSheet("MultiplayerMenu")

function P.onLoad()
    local listbox = winMgr:getWindow("orxonox/MultiplayerLevelListbox")
    local preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
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
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end
    P.multiplayerMode = "startClient"
    if P.multiplayerMode == "startClient" then
        local window = winMgr:getWindow("orxonox/MultiplayerJoinButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
    if P.multiplayerMode == "startServer" then
        local window = winMgr:getWindow("orxonox/MultiplayerHostButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
    if P.multiplayerMode == "startDedicated" then
        local window = winMgr:getWindow("orxonox/MultiplayerDedicatedButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
end

function P.MultiplayerJoinButton_clicked(e)
    P.multiplayerMode = "startClient"
end

function P.MultiplayerHostButton_clicked(e)
    P.multiplayerMode = "startServer"
end

function P.MultiplayerDedicatedButton_clicked(e)
    P.multiplayerMode = "startDedicated"
end

function P.MultiplayerStartButton_clicked(e)
    local choice = winMgr:getWindow("orxonox/MultiplayerLevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.execute(P.multiplayerMode)
        hideAllMenuSheets()
    end
end

function P.MultiplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

