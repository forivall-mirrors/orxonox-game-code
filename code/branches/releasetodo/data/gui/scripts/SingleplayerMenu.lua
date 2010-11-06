-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

function P.onLoad()
    listbox = winMgr:getWindow("orxonox/SingleplayerLevelListbox")
    preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    size = orxonox.LevelManager:getInstance():getNumberOfLevels()
    local levelList = {}
    local index = 0
    local level = ""
    while index < size do
      level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
      table.insert(levelList, level)
      index = index + 1
    end
    table.sort(levelList)
    for k,v in pairs(levelList) do
        item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end

end

function P.SingleplayerStartButton_clicked(e)
    local choice = CEGUI.toListbox(winMgr:getWindow("orxonox/SingleplayerLevelListbox")):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.execute("startGame")
        hideAllMenuSheets()
    end
end

function P.SingleplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

