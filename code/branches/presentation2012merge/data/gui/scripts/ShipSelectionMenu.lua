-- ShipSelectionMenu.lua

local P = createMenuSheet("ShipSelectionMenu")
P.activeTabIndexes = {}
P.scrollbarWidth = 13
P.shipList = {}
function P.onLoad()
    orxonox.execute("set shipmodelsfile [open {../levels/templates/.shipmodels} w+]")
    orxonox.execute("foreach line [glob -directory ../levels/templates/ spaceship*] { regexp {.*/(.*)\\..*} $line matchall match1; puts $shipmodelsfile $match1 }")
    orxonox.execute("close $shipmodelsfile")
    P.createFilterTab("All Ships")
end

function P.createShipList() --generates list with tagged shipmodels
    P.shipList = {}
    file = io.open("../levels/templates/.shipmodels")
    if file ~= nil then
        for line in file:lines() do  --checks if shipmodel is included in level file
            if selectedlevel:hasStartingShip(string.lower(line)) then
                P.shipList[#P.shipList+1] = string.lower(line)
            end
        end
        --file.close() -- TODO: investigate why close() seems to crash?
    end
end

function P.update() --updates listbox with found models
    P.createShipList()
    listbox:resetList()
    --orxonox.GUIManager:setItemTooltipsEnabledHelper(listbox, true)
    local tabIndexes = {}
    for k,v in pairs(P.shipList) do
        --TODO: only add ship if is in the right filter tab
        --if tag == nil or v:hasStartingShip(tag) then
            local item = CEGUI.createListboxTextItem(v)
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            listbox:addItem(item)
            table.insert(tabIndexes, k)
            --orxonox.GUIManager:setTooltipTextHelper(item, v:getDescription())
        --end
    end
    table.insert(P.activeTabIndexes, tabIndexes)
end

function P.createFilterTab(name) -- generates filter tab and list box, sets handler for selection changes
    tabName = "orxonox/ShipSelectionLevelTab"
    -- create new tab window with desired name
    listbox = CEGUI.toListbox(winMgr:createWindow("MenuWidgets/Listbox", tabName))
    listbox:setText(name)
    listbox:setProperty("UnifiedMaxSize", "{{1,0},{1,0}}")
    --[[TODO: smaller list if image and description is implemented.
    listbox:setProperty("UnifiedAreaRect", "{{0.05,0},{0.1,0},{0.5,0},{0.675,0}}") --]]
    listbox:setProperty("UnifiedAreaRect", "{{0,0},{0,0},{1,0},{1,0}}")
    -- fill listbox with items
    P.update()
    -- listen to selection changes
    orxonox.GUIManager:subscribeEventHelper(listbox, "ItemSelectionChanged", P.name..".ShipSelectionSelectionChanged")
    local tabControl = winMgr:getWindow("orxonox/ShipSelectionTabControl")
    orxonox.GUIManager:subscribeEventHelper(tabControl, "TabSelectionChanged", P.name..".ShipSelectionSelectionChanged")
    if listbox:getItemCount() > 0 then
        tabControl:addChildWindow(tabName)
    end
end

function P.ShipSelectionGetSelectedModel() --returns selected model, if available.
    -- choose the active listbox
    local tabControl = CEGUI.toTabControl(winMgr:getWindow("orxonox/ShipSelectionTabControl"))
    local listbox = CEGUI.toListbox(tabControl:getTabContentsAtIndex(tabControl:getSelectedTabIndex()))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        -- get the right tab and the right index
        local tabIndexes = P.activeTabIndexes[tabControl:getSelectedTabIndex()+1]
        local index = tabIndexes[listbox:getItemIndex(choice)+1]
        return P.shipList[index]
    else
        return nil
    end
end

function P.ShipSelectionSelectionChanged(e)
    --[[ TODO: Get image and description from template file
    local levelImage = winMgr:getWindow("orxonox/ShipSelectionLevelImage")
    local levelDescription = winMgr:getWindow("orxonox/ShipSelectionLevelDescription")
    local configButton = winMgr:getWindow("orxonox/ShipSelectionConfigButton")
    local level = P.ShipSelectionGetSelectedModel()
    if level ~= nil then
        --local levelXMLFilename = selectedlevel:getXMLFilename()
        --local imageName = selectedlevel:getScreenshot()
        -- set the screenshot and the description for the selected level
        --levelImage:setProperty("Image", "set:"..levelXMLFilename..imageName.." image:full_image")
        --levelDescription:setText(level:getDescription())
        -- only enable config button for "gametype" levels
        --if level:hasTag("gametype") then
         --   configButton:setProperty("Disabled", "False")
        --else
        --    configButton:setProperty("Disabled", "True")
        --end
    else
        -- also take care of "no level selected"
        levelImage:setProperty("Image", nil)
        levelDescription:setText("")
    end
   --]]
end

function P.ShipSelectionStartButton_clicked(e)

    if (selectedlevel ~= nil and P.ShipSelectionGetSelectedModel() ~= nil)  then
    selectedlevel:selectStartingShip(P.ShipSelectionGetSelectedModel())
        orxonox.execute("startGame " .. "_temp.oxw")
        hideAllMenuSheets()
    else
    orxonox.execute("orxout user_warning no ship model selected or no tagged shipmodel installed")
    end
end

--[[ TODO: function P.ShipSelectionConfigButton_clicked(e)

    local level = P.ShipSelectionGetSelectedModel()
    if level ~= nil then
        local configMenu = showMenuSheet("ShipSelectionConfigMenu")
        configMenu:loadConfig(level)
    end
end
--]]


function P.ShipSelectionBackButton_clicked(e)
    orxonox.execute("keyESC")
end

return P
