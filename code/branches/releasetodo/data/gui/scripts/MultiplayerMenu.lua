-- MultiplayerMenu.lua

local P = createMenuSheet("MultiplayerMenu")

P.levelList = {}
P.itemList = {}
P.showAll = false

function P.onLoad()
    P.multiplayerMode = "startClient"
end

function P.onShow()
    if P.multiplayerMode == "startClient" then
        local window = winMgr:getWindow("orxonox/MultiplayerJoinButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        local checkbox = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
        checkbox:setProperty("Disabled", "True")
        P.showServerList()
    end
    if P.multiplayerMode == "startServer" then
        local window = winMgr:getWindow("orxonox/MultiplayerHostButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        local checkbox = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
        checkbox:setProperty("Disabled", "False")
        P.showLevelList()
    end
    if P.multiplayerMode == "startDedicated" then
        local window = winMgr:getWindow("orxonox/MultiplayerDedicatedButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        local checkbox = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
        checkbox:setProperty("Disabled", "True")
        P.showLevelList()
    end
end

function P.MultiplayerJoinButton_clicked(e)
    P.multiplayerMode = "startClient"
    local checkbox = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
    checkbox:setProperty("Disabled", "True")
    P.showServerList()
end

function P.MultiplayerHostButton_clicked(e)
    P.multiplayerMode = "startServer"
    local checkbox = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
    checkbox:setProperty("Disabled", "False")
    P.showLevelList()
end

function P.MultiplayerDedicatedButton_clicked(e)
    P.multiplayerMode = "startDedicated"
    local checkbox = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
    checkbox:setProperty("Disabled", "True")
    P.showLevelList()
end

function P.MultiplayerShowAll_clicked(e)
    local checkbox = tolua.cast(winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox"), "CEGUI::Checkbox")
    local show = checkbox:isSelected()
    if show ~= P.showAll then
        P.showAll = show
        P.createLevelList()
    end
end

function P.MultiplayerStartButton_clicked(e)
    local choice = winMgr:getWindow("orxonox/MultiplayerListbox"):getFirstSelectedItem()
    if P.multiplayerMode == "startClient" then
        if choice then
            local client = orxonox.Client:getInstance()
            local index = tolua.cast(choice, "CEGUI::ListboxItem"):getID()
            client:setDestination( P.serverList[index][2], 55556 )
        else
            return
        end
    else
        if choice then
            orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        else
            return
        end
    end
    orxonox.execute(P.multiplayerMode)
    hideAllMenuSheets()
end

function P.MultiplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.showLevelList()
    P.createLevelList()
end

function P.createLevelList()
    P.levelList = {}
    P.itemList = {}
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/MultiplayerListbox"))
    listbox:resetList()
    orxonox.GUIManager:setItemTooltipsEnabledHelper(listbox, true)
    local preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    local size = orxonox.LevelManager:getInstance():getNumberOfLevels()
    local index = 0
    local level = nil
    while index < size do
        level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
        if level ~= nil then
            if P.showAll or not level:hasTag("test") then
                table.insert(P.levelList, level)
            end
        end
        index = index + 1
    end
    --TODO: Reintroduce sorting, if needed. At the moment it's sorted by filename.
    --table.sort(levelList)
    for k,v in pairs(P.levelList) do
        local item = CEGUI.createListboxTextItem(v:getName())
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        listbox:addItem(item)
        if v:getXMLFilename() == preselect then
            listbox:setItemSelectState(item, true)
        end
        P.itemList[k] = listbox:getListboxItemFromIndex(k-1)
        orxonox.GUIManager:setTooltipTextHelper(P.itemList[k], v:getDescription())
    end
end

function P.showServerList()
    local listbox = winMgr:getWindow("orxonox/MultiplayerListbox")
    CEGUI.toListbox(listbox):resetList()
    local discovery = orxonox.LANDiscovery:getInstance()
    discovery:discover()
    P.serverList = {}
    local index = 0
    local servername = ""
    local serverip = ""
    while true do
        servername = discovery:getServerListItemName(index)
        if servername == "" then
            break
        end
        serverip = discovery:getServerListItemIP(index)
        if serverip == "" then
          break
        end
        table.insert(P.serverList, {servername, serverip})
        index = index + 1
    end
    index = 1
    for k,v in pairs(P.serverList) do
        local item = CEGUI.createListboxTextItem( v[1] .. ": " .. v[2] )
        item:setID(index)
        index = index + 1
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
    end
end

return P

