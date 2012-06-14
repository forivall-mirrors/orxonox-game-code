-- PickupInventory.lua

local P = createMenuSheet("PickupInventory")

P.wrapper = nil
P.detailsWindows = {}
P.detailPickups = {}
P.pickupsList = {}

P.showing = false

-- Design parameters
P.imageHeight = 50
P.detailImageSize = 100
P.textHeight = 30
P.buttonWidth = 85

function P.onLoad()
    P.wrapper = nil
    P.detailsWindows = {}
    P.detailPickups = {}
    P.pickupsList = {}
end

function P.onShow()
    P.createInventory()
    P.showing = true
end

function P.onHide()
    P.showing = false
    P.cleanup(true)
end

function P.update()
    if P.showing == false then
        return
    end

    -- Update opened detail windows.
    for k,v in pairs(P.detailsWindows) do
        if v ~= nil then
            local pickup = P.detailPickups[k]
            if pickup ~= nil and pickup ~= 0 then
                local useButton = winMgr:getWindow("orxonox/PickupInventory/Details" .. k .. "/UseButton")
                local dropButton = winMgr:getWindow("orxonox/PickupInventory/Details" .. k .. "/DropButton")
                if orxonox.PickupManager:getInstance():isValidPickup(pickup.pickup) == false then
                    useButton:setEnabled(false)
                    dropButton:setEnabled(false)
                    P.detailPickups[k] = nil
                else
                    useButton:setEnabled(true)
                    if pickup.inUse == true then
                        useButton:setText("unuse")
                        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseDetailButton_clicked")
                        if pickup.usable == false then
                            useButton:setEnabled(false)
                        end
                    else
                        useButton:setText("use")
                        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUnuseDetailButton_clicked")
                        if pickup.unusable == false then
                            useButton:setEnabled(false)
                        end
                    end

                    if pickup.pickedUp == false then
                        useButton:setEnabled(false)
                        dropButton:setEnabled(false)
                        P.detailPickups[k] = nil
                    end
                end
            end
        end
    end

    -- Update main inventory.
    P.cleanup(false)
    P.createInventory()
    -- TODO: Recover scrolling position
    
end

function P.createInventory()
    local pickupManager = orxonox.PickupManager:getInstance()
    
    local root = winMgr:getWindow("orxonox/PickupInventory/Inventory")
    P.wrapper = winMgr:createWindow("MenuWidgets/ScrollablePane", "orxonox/PickupInventory/Inventory/Wrapper")
    P.wrapper:setSize(CEGUI.UVector2(CEGUI.UDim(1,0),CEGUI.UDim(1,0)))
    root:addChildWindow(P.wrapper)
    
    P.pickupsList = {}

    local numPickups = pickupManager:getNumPickups()
    local counter = 1
    local offset = 0
    while counter <= numPickups do
        local pickup = pickupManager:popPickup()
        table.insert(P.pickupsList, pickup)
        local window = P.createPickupEntry(counter, pickup)
        window:setYPosition(CEGUI.UDim(0,offset))
        offset = offset + window:getHeight():asAbsolute(1)
        P.wrapper:addChildWindow(window)
        counter = counter + 1
    end

end

function P.createPickupEntry(index, pickup)
    local representation = orxonox.PickupManager:getInstance():getPickupRepresentation(pickup.pickup)

    local name = "orxonox/PickupInventory/Box/Pickup" .. index

    local item = winMgr:createWindow("MenuWidgets/StaticText", name)
    item:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, P.imageHeight)))
    item:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))

    local image = winMgr:createWindow("MenuWidgets/StaticImage", name .. "/Image")
    image:setProperty("Image", "set:PickupInventory image:" .. representation:getInventoryRepresentation())
    image:setProperty("BackgroundEnabled", "set:False")
    image:setProperty("FrameEnabled", "set:True")
    image:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.imageHeight), CEGUI.UDim(0, P.imageHeight)))
    item:addChildWindow(image)

    local title = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Title")
    title:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.imageHeight+5), CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    title:setSize(CEGUI.UVector2(CEGUI.UDim(0.3, 0), CEGUI.UDim(0, P.textHeight)))
    title:setText(representation:getPickupName())
    title:setProperty("FrameEnabled", "set:False")
    item:addChildWindow(title)

    local useButton = winMgr:createWindow("MenuWidgets/Button", name .. "/UseButton")
    useButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.3, P.imageHeight+10),CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    useButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    if pickup.inUse == false then
        useButton:setText("use")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseButton_clicked")
        if pickup.usable == false then
            useButton:setEnabled(false)
        end
    else
        useButton:setText("unuse")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUnuseButton_clicked")
        if pickup.unusable == false then
            useButton:setEnabled(false)
        end
    end
    item:addChildWindow(useButton)

    local dropButton = winMgr:createWindow("MenuWidgets/Button", name .. "/DropButton")
    dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.3, P.imageHeight+15+P.buttonWidth),CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    dropButton:setText("drop")
    orxonox.GUIManager:subscribeEventHelper(dropButton, "Clicked", P.name .. ".InventoryDropButton_clicked")
    item:addChildWindow(dropButton)

    local detailsButton = winMgr:createWindow("MenuWidgets/Button", name .. "/DetailsButton")
    detailsButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.3, P.imageHeight+20+2*P.buttonWidth),CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    detailsButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    detailsButton:setText("details")
    orxonox.GUIManager:subscribeEventHelper(detailsButton, "Clicked", P.name .. ".InventoryDetailsButton_clicked")
    item:addChildWindow(detailsButton)

    return item
end

function P.cleanup(destroyDetails)
    if P.wrapper ~= nil then
        winMgr:destroyWindow(P.wrapper)
    end
    
    --Destroy details windows.
    if destroyDetails == false then
        return
    end
    for k,v in pairs(P.detailsWindows) do
        if v ~= nil then
            P.destroyDetailWindow(k)
        end
    end
end

function P.windowToPickupHelper(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local pickupIndex = tonumber(match())

    return pickupIndex
end

function P.createDetailsWindow(pickupIndex)
    local pickup = P.pickupsList[pickupIndex]
    local representation = orxonox.PickupManager:getInstance():getPickupRepresentation(pickup.pickup)

    local index = P.getNewDetailNumber()
    local name = "orxonox/PickupInventory/Details" .. index
    
    local window = winMgr:createWindow("MenuWidgets/FrameWindow", name)
    window:setSize(CEGUI.UVector2(CEGUI.UDim(0.5,0),CEGUI.UDim(0.4,0)))
    orxonox.GUIManager:subscribeEventHelper(window, "CloseClicked", P.name .. ".closeDetailWindow")
    
    local root = winMgr:getWindow("orxonox/PickupInventory/Background")
    root:addChildWindow(window)
    
    local wrapper = winMgr:createWindow("DefaultWindow", name .. "/Wrapper")
    wrapper:setSize(CEGUI.UVector2(CEGUI.UDim(1, -20),CEGUI.UDim(1, -50)))
    wrapper:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10),CEGUI.UDim(0, 40)))
    window:addChildWindow(wrapper)
    
    local title = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Title")
    title:setText(representation:getPickupName())
    title:setHeight(CEGUI.UDim(0, P.textHeight))
    title:setProperty("FrameEnabled", "set:False")
    title:setProperty("BackgroundEnabled", "set:False")
    wrapper:addChildWindow(title)
    
    local image = winMgr:createWindow("MenuWidgets/StaticImage", name .. "/Image")
    image:setProperty("Image", "set:PickupInventory image:" .. representation:getInventoryRepresentation())
    image:setProperty("BackgroundEnabled", "set:False")
    image:setProperty("FrameEnabled", "set:True")
    image:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.detailImageSize), CEGUI.UDim(0, P.detailImageSize)))
    image:setYPosition(CEGUI.UDim(0, P.textHeight + 5))
    wrapper:addChildWindow(image)
    
    local box = winMgr:createWindow("MenuWidgets/ScrollablePane", name .. "/Description")
    box:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -1*(P.detailImageSize + 10)),CEGUI.UDim(1, -(P.textHeight + 5 + P.textHeight + 20))))
    box:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.detailImageSize + 10),CEGUI.UDim(0, P.textHeight + 5)))
    local description = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Description/Text")
    description:setText(representation:getPickupDescription())
    description:setProperty("HorzFormatting", "WordWrapLeftAligned")
    description:setProperty("VertFormatting", "TopAligned")
    box:addChildWindow(description)
    wrapper:addChildWindow(box)

    local useButton = winMgr:createWindow("MenuWidgets/Button", name .. "/UseButton")
    useButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.detailImageSize+10),CEGUI.UDim(1, -40)))
    useButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    if pickup.inUse == false then
        useButton:setText("use")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseDetailButton_clicked")
        if pickup.usable == false then
            useButton:setEnabled(false)
        end
    else
        useButton:setText("unuse")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUnuseDetailButton_clicked")
        if pickup.unusable == false then
            useButton:setEnabled(false)
        end
    end
    wrapper:addChildWindow(useButton)
    
    local dropButton = winMgr:createWindow("MenuWidgets/Button", name .. "/DropButton")
    dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.detailImageSize+10+P.buttonWidth+10),CEGUI.UDim(1, -40)))
    dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    dropButton:setText("drop")
    orxonox.GUIManager:subscribeEventHelper(dropButton, "Clicked", P.name .. ".InventoryDropDetailButton_clicked")
    wrapper:addChildWindow(dropButton)

    P.detailsWindows[index] = window
    P.detailPickups[index] = pickup
    
end

function P.getNewDetailNumber()
    local number = table.getn(P.detailsWindows)
    for k,v in pairs(P.detailsWindows) do
        if v == nil then
            number = k-1
        end
    end
    return number+1
end

function P.InventoryUseButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    local pickup = P.pickupsList[pickupIndex]
    orxonox.PickupManager:getInstance():usePickup(pickup.pickup, true)
end

function P.InventoryUnuseButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    local pickup = P.pickupsList[pickupIndex]
    orxonox.PickupManager:getInstance():usePickup(pickup.pickup, false)
end

function P.InventoryDropButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    local pickup = P.pickupsList[pickupIndex]
    orxonox.PickupManager:getInstance():dropPickup(pickup.pickup)
end

function P.InventoryDetailsButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    P.createDetailsWindow(pickupIndex)
end

function P.InventoryUseDetailButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    local pickup = P.detailPickups[pickupIndex]
    orxonox.PickupManager:getInstance():usePickup(pickup.pickup, true)
end

function P.InventoryUnuseDetailButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    local pickup = P.detailPickups[pickupIndex]
    orxonox.PickupManager:getInstance():usePickup(pickup.pickup, false)
end

function P.InventoryDropDetailButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    local pickup = P.detailPickups[pickupIndex]
    orxonox.PickupManager:getInstance():dropPickup(pickup.pickup)
end

function P.closeDetailWindow(e)
    --Get some numbers from the window
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    local detailNr = tonumber(match())
    
    P.destroyDetailWindow(detailNr)
end

function P.destroyDetailWindow(detailNr)
    local window = P.detailsWindows[detailNr]
    winMgr:destroyWindow(window)
    P.detailsWindows[detailNr] = nil
    P.detailPickups[detailNr] = nil
end

function P.InventoryBackButton_clicked(e)
    orxonox.CommandExecutor:execute("OrxonoxOverlay toggleVisibility PickupInventory")
end

return P
