-- ShipSelectionMenu.lua

local P = createMenuSheet("ShipSelectionMenu")
P.activeTabIndexes = {}
P.scrollbarWidth = 13
P.shipList = {}
function P.onLoad()
   --orxonox.execute("orxout user_warning Ships= " .. selectedlevel:hasShip("abcdef"))
   local dircmd = "ls ../levels/templates/ -l | awk '{print $9}' | grep \"spaceship\" | sed -e 's/\\.[a-zA-Z]*$//'" -- go to spaceships folder and generate a list of installed shipmodels.
   os.execute(dircmd .. " > ../levels/templates/.shipmodels") --saves output in a textfile
   --[[ Program a Windows Version here:
   if string.sub(package.config,1,1) == '\\' then
           -- Windows
           dircmd = "dir /b/s"
   end]]	

   P.shipList = {}
   for line in io.lines("../levels/templates/.shipmodels") do  
	if selectedlevel:hasShip(string.lower(line)) then
		P.shipList[#P.shipList+1] = string.lower(line)
	end
   end 
   P.createFilterTab("Show All")

end

function P.createShipList()
       

end



function P.createFilterTab(name)

    local tabName = "orxonox/ShipSelectionLevelTab"
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
    for k,v in pairs(P.shipList) do
        -- only add level if it has desired tag
        --if tag == nil or v:hasShip(tag) then
            local item = CEGUI.createListboxTextItem(v)
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            listbox:addItem(item)
            table.insert(tabIndexes, k)
            --[[if v:getXMLFilename() == preselect then
                listbox:setItemSelectState(item, true)
            end--]]
            --orxonox.GUIManager:setTooltipTextHelper(item, v:getDescription())
        --end
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

function P.ShipSelectionGetSelectedModel()
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
        configButton:setProperty("Disabled", "True")
    end
end

function P.ShipSelectionStartButton_clicked(e)

    if selectedlevel ~= nil then
	selectedlevel:selectShip(P.ShipSelectionGetSelectedModel())
        orxonox.execute("startGame " .. "_temp.oxw")
        hideAllMenuSheets()
    else
        orxonox.execute("keyESC")
    end
end

function P.ShipSelectionConfigButton_clicked(e)
--[[
    local level = P.ShipSelectionGetSelectedModel()
    if level ~= nil then
        local configMenu = showMenuSheet("ShipSelectionConfigMenu")
        configMenu:loadConfig(level)
    end
--]]
end

function P.ShipSelectionBackButton_clicked(e)
    --hideAllMenuSheets()
    orxonox.execute("keyESC")
end 

return P
