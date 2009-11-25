-- NewSingleplayerMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewSingleplayerMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewSingleplayerMenu"
P.layoutString = "NewSingleplayerMenu.layout"

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
    -- start game
    debug("event: start")
end

function P.SingleplayerBackButton_clicked(e)
    hideGUI("NewSingleplayerMenu")
    debug("event: back")
end

return P

