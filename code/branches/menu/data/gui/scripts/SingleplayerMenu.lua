-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

P.buttonList = {}

function P.onLoad()
    listbox = winMgr:getWindow("orxonox/SingleplayerLevelListbox")
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
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end

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

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
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

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 1, 2)
end

return P

