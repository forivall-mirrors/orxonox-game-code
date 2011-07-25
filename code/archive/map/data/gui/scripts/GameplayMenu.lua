-- GameplayMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    GameplayMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "GameplayMenu"
P.layoutString = "GameplayMenu.layout"

function P:init()
    dropdown = winMgr:getWindow("orxonox/ThemeCombobox")
    local themeList = {}
    table.insert(themeList, "Theme 1")
    table.insert(themeList, "Theme 2")
    table.insert(themeList, "Theme 3")
    for k,v in pairs(themeList) do
        item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toCombobox(dropdown):addItem(item)
    end
end

function P.GameplayThemeCombobox_changed(e)
    -- theme
    debug("event: theme")
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
    hideGUI(P.filename)
end

return P

