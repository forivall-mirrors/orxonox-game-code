-- HostMenu.lua

local P = createMenuSheet("HostMenu")

P.multiplayerMode = "startServer"

function P.onShow()
    if P.multiplayerMode == "startServer" then
        local window = winMgr:getWindow("orxonox/HostMenuHostButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.showLevelList()
    end

    if P.multiplayerMode == "startDedicated" then
        local window = winMgr:getWindow("orxonox/HostMenuDedicatedButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.showLevelList()
    end

end

function P.HostMenuBuildServerButton_clicked(e)
    P.multiplayerMode = "startServer"
    P.showLevelList()
end

function P.HostMenuDedicatedButton_clicked(e)
    P.multiplayerMode = "startDedicated"
    P.showLevelList()
end

function P.HostMenuBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.HostMenuStartButton_clicked(e)
    local choice = winMgr:getWindow("orxonox/HostMenuListbox"):getFirstSelectedItem()
        if choice then
            orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        else
            return
        end
    orxonox.execute(P.multiplayerMode)
    hideAllMenuSheets()
end



function P.showLevelList()
    local listbox = winMgr:getWindow("orxonox/HostMenuListbox")
    CEGUI.toListbox(listbox):resetList()
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
    index = 1
    for k,v in pairs(levelList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        item:setID(index)
        index = index + 1
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end
end



return P
