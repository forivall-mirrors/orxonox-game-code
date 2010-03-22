-- SingleplayerMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    SingleplayerMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "SingleplayerMenu"
P.layoutString = "SingleplayerMenu.layout"

function P:init()
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
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end
end

function P.SingleplayerStartButton_clicked(e)
    choice = winMgr:getWindow("orxonox/SingleplayerLevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.execute("startGame")
        hideAllGUIs()
    end
end

function P.SingleplayerBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

