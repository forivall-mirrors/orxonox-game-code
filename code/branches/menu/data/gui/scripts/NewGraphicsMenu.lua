-- NewGraphicsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewGraphicsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewGraphicsMenu"
P.layoutString = "NewGraphicsMenu.layout"

function P:init()
    dropdown = winMgr:getWindow("orxonox/ResolutionCombobox")
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

function P.GraphicsFullscreenCheckbox_clicked(e)
    -- fullscreen
    debug("event: fullscreen")
end

function P.GraphicsBrightnessScrollbar_clicked(e)
    -- brightness
    debug("event: brightness")
end

function P.GraphicsBackButton_clicked(e)
    hideGUI("NewGraphicsMenu")
    debug("event: back")
end

return P

