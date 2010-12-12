-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

P.buttonList = {}
P.levelList = {}
P.itemList = {}
P.showAll = false

function P.onLoad()
    local window = winMgr:getWindow("orxonox/SingleplayerShowAllCheckbox")
    local button = tolua.cast(window,"CEGUI::Checkbox")
    button:setSelected(false)
    P.createLevelList()

    --buttons are arranged in a 1x2 matrix
    local item = {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerStartButton"),
            ["function"]  = P.SingleplayerStartButton_clicked
    }
    P.buttonList[1] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerBackButton"),
            ["function"]  = P.SingleplayerBackButton_clicked
    }
    P.buttonList[2] = item
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

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
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

function P.SingleplayerShowAll_clicked(e)
    local checkbox = tolua.cast(winMgr:getWindow("orxonox/SingleplayerShowAllCheckbox"), "CEGUI::Checkbox")
    local show = checkbox:isSelected()
    if show ~= P.showAll then
        P.showAll = show
        P.createLevelList()
   end
end

function P.SingleplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 1, 2)
end

return P

