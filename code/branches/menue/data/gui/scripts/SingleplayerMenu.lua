-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

P.levelList = {}
P.itemList = {}

function P.onLoad()
    P.createLevelList(nil)

    --buttons are arranged in a 1x2 matrix
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerStartButton"),
            ["callback"]  = P.SingleplayerStartButton_clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerBackButton"),
            ["callback"]  = P.SingleplayerBackButton_clicked
    })
end

function P.createLevelList(tag)
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
            if tag == nil then
                table.insert(P.levelList, level)
            elseif level:hasTag(tag) then
                table.insert(P.levelList, level)
            end
        end
        index = index + 1
    end

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

function P.SingleplayerStartButton_clicked(e)
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/SingleplayerLevelListbox"))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        local index = listbox:getItemIndex(choice)
        local level = P.levelList[index+1]
        if level ~= nil then
            orxonox.execute("startGame " .. level:getXMLFilename())
            hideAllMenuSheets()
        end
    end
end

function P.SingleplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

