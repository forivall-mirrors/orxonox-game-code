-- ShipSelectionMenu.lua

local P = createMenuSheet("ShipSelectionMenu")
level = nil
P.activeTabIndexes = {}
P.scrollbarWidth = 13
function P.loadShips(levelname)
    --orxonox.execute("echo " .. levelname)
    --level = levelname
end
function P.onLoad()
--[[    P.createLevelList()
    
    -- create tabs with desired tab as argument (nil for all)
    P.createFilterTab("Gametypes", "gametype")
    P.createFilterTab("Missions", "mission")
    P.createFilterTab("Minigames", "minigame")
    P.createFilterTab("Showcases", "showcase")
    P.createFilterTab("Presentations", "presentation")
    P.createFilterTab("Tests", "test")
    P.createFilterTab("Show All", nil)
    
    -- update description and screenshot boxes
    P.ShipSelectionSelectionChanged()
    
    --buttons are arranged in a 1x3 matrix
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/ShipSelectionStartButton"),
            ["callback"]  = P.ShipSelectionStartButton_clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/ShipSelectionConfigButton"),
            ["callback"]  = P.ShipSelectionConfigButton_clicked
    })

    P:setButton(1, 3, {
            ["button"] = winMgr:getWindow("orxonox/ShipSelectionBackButton"),
            ["callback"]  = P.ShipSelectionBackButton_clicked
    })--]]
end
--[[
function P.createLevelList()
    P.levelList = {}
    local size = orxonox.LevelManager:getInstance():getNumberOfLevels()
    local index = 0
    local level = nil
    while index < size do
        level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
        if level ~= nil then
            local levelXMLFilename = level:getXMLFilename()
            -- create an imageset for each screenshot
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
    local tabName = "orxonox/ShipSelectionLevelTab"
    if tag ~= nil then
        tabName = tabName..tag
    end
    -- create new tab window with desired name
    local listbox = CEGUI.toListbox(winMgr:createWindow("MenuWidgets/Listbox", tabName))
    listbox:setText(name)
    listbox:setProperty("UnifiedMaxSize", "{{1,0},{1,0}}")
    listbox:setProperty("UnifiedAreaRect", "{{0.05,0},{0.1,0},{0.5,0},{0.675,0}}")
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
    -- listen to selection changes
    orxonox.GUIManager:subscribeEventHelper(listbox, "ItemSelectionChanged", P.name..".ShipSelectionSelectionChanged")
    local tabControl = winMgr:getWindow("orxonox/ShipSelectionTabControl")
    orxonox.GUIManager:subscribeEventHelper(tabControl, "TabSelectionChanged", P.name..".ShipSelectionSelectionChanged")
    if listbox:getItemCount() > 0 then
        tabControl:addChildWindow(tabName)
    end
end

function P.ShipSelectionGetSelectedLevel()
    -- choose the active listbox
    local tabControl = CEGUI.toTabControl(winMgr:getWindow("orxonox/ShipSelectionTabControl"))
    local listbox = CEGUI.toListbox(tabControl:getTabContentsAtIndex(tabControl:getSelectedTabIndex()))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        -- get the right tab and the right index
        local tabIndexes = P.activeTabIndexes[tabControl:getSelectedTabIndex()+1]
        local index = tabIndexes[listbox:getItemIndex(choice)+1]
        return P.levelList[index]
    else
        return nil
    end
end

function P.ShipSelectionSelectionChanged(e)
    local levelImage = winMgr:getWindow("orxonox/ShipSelectionLevelImage")
    local levelDescription = winMgr:getWindow("orxonox/ShipSelectionLevelDescription")
    local configButton = winMgr:getWindow("orxonox/ShipSelectionConfigButton")
    local level = P.ShipSelectionGetSelectedLevel()
    if level ~= nil then
        local levelXMLFilename = level:getXMLFilename()
        local imageName = level:getScreenshot()
        -- set the screenshot and the description for the selected level
        levelImage:setProperty("Image", "set:"..levelXMLFilename..imageName.." image:full_image")
        levelDescription:setText(level:getDescription())
        -- only enable config button for "gametype" levels
        if level:hasTag("gametype") then
            configButton:setProperty("Disabled", "False")
        else
            configButton:setProperty("Disabled", "True")
        end
    else
        -- also take care of "no level selected"
        levelImage:setProperty("Image", nil)
        levelDescription:setText("")
        configButton:setProperty("Disabled", "True")
    end
end
--]]
function P.ShipSelectionStartButton_clicked(e)

    if P.level ~= nil then
        orxonox.execute("startGame " .. level:getXMLFilename())
        hideAllMenuSheets()
    else
        orxonox.execute("keyESC")
    end
end

function P.ShipSelectionConfigButton_clicked(e)
--[[
    local level = P.ShipSelectionGetSelectedLevel()
    if level ~= nil then
        local configMenu = showMenuSheet("ShipSelectionConfigMenu")
        configMenu:loadConfig(level)
    end
--]]
end

function P.ShipSelectionBackButton_clicked(e)
    orxonox.execute("keyESC")
end 

return P
