-- GameplayMenu.lua

local P = createMenuSheet("GameplayMenu")

function P.onLoad()
    dropdown = winMgr:getWindow("orxonox/ThemeCombobox")
    local themeList = {}
    table.insert(themeList, "Theme 1")
    table.insert(themeList, "Theme 2")
    table.insert(themeList, "Theme 3")
    for k,v in pairs(themeList) do
        item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toCombobox(dropdown):addItem(item)
    end
end

function P.GameplayThemeCombobox_changed(e)
    -- theme
    logMessage(0, "event: theme")
end

function P.GameplayDifficultyEasyButton_clicked(e)
    -- difficulty easy
    logMessage(0, "event: easy")
end

function P.GameplayDifficultyNormalButton_clicked(e)
    -- difficulty normal
    logMessage(0, "event: normal")
end

function P.GameplayDifficultyHardButton_clicked(e)
    -- difficulty hard
    logMessage(0, "event: hard")
end

function P.GameplayBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

