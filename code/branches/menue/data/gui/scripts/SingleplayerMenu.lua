-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerMenu")

P.levelList = {}
P.activeTabIndexes = {}
P.scrollbarWidth = 13

function P.onLoad()
    P.createLevelList()
    
    -- create tabs with desired tab as argument (nil for all)
    P.createFilterTab("Tests", "test")
    P.createFilterTab("Tutorials", "tutorial")
    P.createFilterTab("Showcases", "showcase")
    P.createFilterTab("SP?", "singleplayer")
    P.createFilterTab("Presentations", "presentation")
    P.createFilterTab("Show All", nil)

    --buttons are arranged in a 1x3 matrix
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerStartButton"),
            ["callback"]  = P.SingleplayerStartButton_clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerConfigButton"),
            ["callback"]  = P.SingleplayerConfigButton_clicked
    })

    P:setButton(1, 3, {
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
            local levelXMLFilename = level:getXMLFilename()
            local imageName = level:getScreenshot()
            if imageName ~= "" then
                CEGUI.ImagesetManager:getSingleton():createImagesetFromImageFile(levelXMLFilename..imageName, imageName)
            else
                CEGUI.ImagesetManager:getSingleton():createImagesetFromImageFile(levelXMLFilename..imageName, "noscreenshot.png")
            end
            table.insert(P.levelList, level)
        end
        index = index + 1
    end
end

function P.createFilterTab(name, tag)
    -- create unique tab window name
    local tabName = "orxonox/SingleplayerLevelTab"
    if tag ~= nil then
        tabName = tabName..tag
    end
    -- add new tab window with desired name
    local tabControl = winMgr:getWindow("orxonox/SingleplayerTabControl")
    local listbox = CEGUI.toListbox(winMgr:createWindow("MenuWidgets/Listbox", tabName))
    listbox:setText(name)
    listbox:setProperty("UnifiedMaxSize", "{{1,0},{1,0}}")
    listbox:setProperty("UnifiedAreaRect", "{{0.05,0},{0.1,0},{0.5,0},{0.7,0}}")
    tabControl:addChildWindow(tabName)
    -- fill listbox with items
    listbox:resetList()
    orxonox.GUIManager:setItemTooltipsEnabledHelper(listbox, true)
    local preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    local tabIndexes = {}
    for k,v in pairs(P.levelList) do
        -- only add level if it has desired tag
        if tag == nil or v:hasTag(tag) then
            local item = CEGUI.createListboxTextItem(v:getName())
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            listbox:addItem(item)
            table.insert(tabIndexes, k)
            if v:getXMLFilename() == preselect then
                listbox:setItemSelectState(item, true)
            end
            orxonox.GUIManager:setTooltipTextHelper(item, v:getDescription())
        end
    end
    table.insert(P.activeTabIndexes, tabIndexes)
    listbox:subscribeEvent("ItemSelectionChanged", "SingleplayerSelectionChanged")
    tabControl:subscribeEvent("TabSelectionChanged", "SingleplayerSelectionChanged")
    SingleplayerSelectionChanged()
end

function P.SingleplayerGetSelectedLevel()
    -- choose the active listbox
    local tabControl = CEGUI.toTabControl(winMgr:getWindow("orxonox/SingleplayerTabControl"))
    local listbox = CEGUI.toListbox(tabControl:getTabContentsAtIndex(tabControl:getSelectedTabIndex()))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        local tabIndexes = P.activeTabIndexes[tabControl:getSelectedTabIndex()+1]
        local index = tabIndexes[listbox:getItemIndex(choice)+1]
        return P.levelList[index]
    else
        return nil
    end
end

function SingleplayerSelectionChanged(e)
    local level = P.SingleplayerGetSelectedLevel()
    if level ~= nil then
        local levelXMLFilename = level:getXMLFilename()
        local imageName = level:getScreenshot()
        local levelImage = winMgr:getWindow("orxonox/SingleplayerLevelImage")
        levelImage:setProperty("Image", "set:"..levelXMLFilename..imageName.." image:full_image")
        local levelDescription = winMgr:getWindow("orxonox/SingleplayerLevelDescription")
        local height = getStaticTextWindowHeight(levelDescription)
--        local width = getStaticTextWindowWidth(levelDescription)
        levelDescription:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(0.0, height)))
        levelDescription:setText(level:getDescription())
    end
end

function P.SingleplayerStartButton_clicked(e)
    local level = P.SingleplayerGetSelectedLevel()
    if level ~= nil then
        orxonox.execute("startGame " .. level:getXMLFilename())
        hideAllMenuSheets()
    end
end

function P.SingleplayerConfigButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.SingleplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

