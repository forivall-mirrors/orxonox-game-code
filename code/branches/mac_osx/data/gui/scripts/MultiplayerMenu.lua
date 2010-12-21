-- MultiplayerMenu.lua

local P = createMenuSheet("MultiplayerMenu")

function P.onLoad()
    P.multiplayerMode = "startClient"
end

function P.onShow()
    if P.multiplayerMode == "startClient" then
        local window = winMgr:getWindow("orxonox/MultiplayerJoinButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.showServerList()
    end
    if P.multiplayerMode == "startServer" then
        local window = winMgr:getWindow("orxonox/MultiplayerHostButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.showLevelList()
    end
    if P.multiplayerMode == "startDedicated" then
        local window = winMgr:getWindow("orxonox/MultiplayerDedicatedButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.showLevelList()
    end
end

function P.MultiplayerJoinButton_clicked(e)
    P.multiplayerMode = "startClient"
    P.showServerList()
end

function P.MultiplayerHostButton_clicked(e)
    P.multiplayerMode = "startServer"
    P.showLevelList()
end

function P.MultiplayerDedicatedButton_clicked(e)
    P.multiplayerMode = "startDedicated"
    P.showLevelList()
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
    local listbox = winMgr:getWindow("orxonox/MultiplayerListbox")
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

