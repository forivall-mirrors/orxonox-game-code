-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

P.levelList = {}
P.itemList = {}
P.showAll = false

function P.onLoad()
    local window = winMgr:getWindow("orxonox/SingleplayerShowRestrictedButton")
    local button = tolua.cast(window,"CEGUI::RadioButton")
    button:setSelected(true)
    P.createLevelList()
end

function P.createLevelList()
    P.levelList = {}
    P.itemList = {}
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/SingleplayerLevelListbox"))
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
    --TODO: Reintroduce sorting, if needed.
    --table.sort(levelList)
    for k,v in pairs(P.levelList) do
        local item = CEGUI.createListboxTextItem(v:getName())
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        listbox:addItem(item)
        if v:getXMLFilename() == preselect then
            listbox:setItemSelectState(item, true)
        end
        P.itemList[k] = listbox:getListboxItemFromIndex(k-1)
        --TODO: The description as tooltip would be nice.
        --local lItem = tolua.cast("CEGUI::ListboxItem", P.itemList[k])
        --lItem:setTooltipText(v:getDescription())
        orxonox.GUIManager:setTooltipTextHelper(P.itemList[k], v:getDescription())
    end
end

function P.SingleplayerStartButton_clicked(e)
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/SingleplayerLevelListbox"))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        local index = listbox:getItemIndex(choice)
        local level = P.levelList[index+1]
        if level ~= nil then
            orxonox.LevelManager:getInstance():setDefaultLevel(level:getXMLFilename())
            orxonox.execute("startGame")
            hideAllMenuSheets()
        end
    end
end

function P.SingleplayerShowRestrictedButton_clicked(e)
    P.showAll = false
    P.createLevelList()
end

function P.SingleplayerShowAllButton_clicked(e)
    P.showAll = true
    P.createLevelList()
end

function P.SingleplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

