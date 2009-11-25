-- NewGameplayMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewGameplayMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewGameplayMenu"
P.layoutString = "NewGameplayMenu.layout"

function P:init()
    dropdown = winMgr:getWindow("orxonox/ThemeCombobox")
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
        CEGUI.toCombobox(dropdown):addItem(item)
        if v .. ".oxw" == preselect then
            dropdown:setItemSelectState(item, true)
        end
    end
end

function P.GameplayDifficultyEasyButton_clicked(e)
    -- difficulty easy
    debug("event: easy")
end

function P.GameplayDifficultyNormalButton_clicked(e)
    -- difficulty normal
    debug("event: normal")
end

function P.GameplayDifficultyHardButton_clicked(e)
    -- difficulty hard
    debug("event: hard")
end

function P.GameplayBackButton_clicked(e)
    hideGUI("NewGameplayMenu")
    debug("event: back")
end

return P

