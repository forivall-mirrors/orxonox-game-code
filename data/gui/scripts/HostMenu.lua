-- HostMenu.lua

local P = createMenuSheet("HostMenu")

P.multiplayerMode = "startServer"

P.levelList = {}
P.itemList = {}
P.showAll = false

function P.onLoad()
    P.multiplayerMode = "startServer"
    local window = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
    local button = tolua.cast(window,"CEGUI::Checkbox")
    button:setSelected(false)
    P.createLevelList()

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/HostMenuStartButton"),
            ["callback"]  = P.HostMenuStartButton_clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/HostMenuBackButton"),
            ["callback"]  = P.HostMenuBackButton_clicked
    })
end

function P.onShow()
    if P.showAll ~= orxonox.GUIManager:inDevMode() then
        local window = winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox")
        local button = tolua.cast(window,"CEGUI::Checkbox")
        --P.showAll = not P.showAll
        button:setSelected(P.showAll)
    end

    if P.multiplayerMode == "startServer" then
        local window = winMgr:getWindow("orxonox/HostMenuHostButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.createLevelList()
    end

    if P.multiplayerMode == "startDedicated" then
        local window = winMgr:getWindow("orxonox/HostMenuDedicatedButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
        P.createLevelList()
    end
end

function P.createLevelList()
    P.levelList = {}
    P.itemList = {}
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/HostMenuListbox"))
    listbox:resetList()
    orxonox.GUIManager:setItemTooltipsEnabledHelper(listbox, true)
    local preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    local size = orxonox.LevelManager:getInstance():getNumberOfLevels()
    local index = 0
    local level = nil
    while index < size do
        level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
        if level ~= nil then
            if P.showAll or not level:hasTag("test") and not level:hasTag("singleplayer") then
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

function P.HostMenuBuildServerButton_clicked(e)
    P.multiplayerMode = "startServer"
    P.createLevelList()
end

function P.HostMenuDedicatedButton_clicked(e)
    P.multiplayerMode = "startDedicated"
    P.createLevelList()
end

function P.HostMenuBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.HostMenuStartButton_clicked(e)
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/HostMenuListbox"))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        local index = listbox:getItemIndex(choice)
        local level = P.levelList[index+1]
        if level ~= nil then
            orxonox.execute(P.multiplayerMode .. " " .. level:getXMLFilename())
            hideAllMenuSheets()
        end
    end
end

function P.MultiplayerShowAll_clicked(e)
    local checkbox = tolua.cast(winMgr:getWindow("orxonox/MultiplayerShowAllCheckbox"), "CEGUI::Checkbox")
    local show = checkbox:isSelected()
    if show ~= P.showAll then
        P.showAll = show
        P.createLevelList()
   end
end

return P
