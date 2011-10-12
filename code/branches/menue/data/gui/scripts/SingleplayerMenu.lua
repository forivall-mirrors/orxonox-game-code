-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

P.levelList = {}

function P.onLoad()
    P.createLevelList()
    
    -- create tabs with desired tab as argument (nil for all)
    P.createFilterTab("Show All", nil)
    P.createFilterTab("Tests", "test")
    P.createFilterTab("Tutorials", "tutorial")
    P.createFilterTab("Showcases", "showcase")
    P.createFilterTab("SP?", "singleplayer")
    P.createFilterTab("Presentations", "presentation")

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

function P.createLevelList()
    P.levelList = {}
    local size = orxonox.LevelManager:getInstance():getNumberOfLevels()
    local index = 0
    local level = nil
    while index < size do
        level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
        if level ~= nil then
            table.insert(P.levelList, level)
        end
        index = index + 1
    end
end

function P.createFilterTab(name, tag)
    -- create unique tab window name
    local tabName = "orxonox/SingleplayerLevelListbox"
    if tag ~= nil then
        tabName = tabName..tag
    end
    -- add new tab window with desired name
    local tabControl = winMgr:getWindow("orxonox/SingleplayerTabControl")
    local newWindow = winMgr:createWindow("MenuWidgets/Listbox", tabName)
    newWindow:setText(name)
    newWindow:setProperty("UnifiedMaxSize", "{{1,0},{1,0}}")
    newWindow:setProperty("UnifiedAreaRect", "{{0.05,0},{0.15,0},{0.95,0},{0.85,0}}")
    tabControl:addChildWindow(tabName)
    -- fill listbox with items
    local listbox = CEGUI.toListbox(newWindow)
    listbox:resetList()
    orxonox.GUIManager:setItemTooltipsEnabledHelper(listbox, true)
    local preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    for k,v in pairs(P.levelList) do
        -- only add level if it has desired tag
        if tag == nil or v:hasTag(tag) then
            local item = CEGUI.createListboxTextItem(v:getName())
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            listbox:addItem(item)
            if v:getXMLFilename() == preselect then
                listbox:setItemSelectState(item, true)
            end
            orxonox.GUIManager:setTooltipTextHelper(item, v:getDescription())
        end
    end
end

function P.SingleplayerStartButton_clicked(e)
    -- choose the active listbox
    local tabControl = CEGUI.toTabControl(winMgr:getWindow("orxonox/SingleplayerTabControl"))
    local listbox = CEGUI.toListbox(tabControl:getTabContentsAtIndex(tabControl:getSelectedTabIndex()))
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

