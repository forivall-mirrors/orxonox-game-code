-- PickupInventory.lua

local P = createMenuSheet("PickupInventory")

P.carrierList = {}
P.wrapper = nil
P.detailsWindows = {}
P.pickupsList = {}

P.showing = false

-- Design parameters
P.imageHeight = 50
P.detailImageSize = 100
P.textHeight = 30
P.buttonWidth = 85

function P.onLoad()
    carrierList = {}
end

function P.onShow()
    P.createInventory()
    P.showing = true
end

function P.onHide()
    P.showing = false
    P.cleanup()
end

function P.update()
    if P.showing == false then
        return
    end
    
    P.cleanup()
    
    P.createInventory()
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
    local representation = orxonox.PickupManager:getInstance():getPickupRepresentation(pickup)

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
    title:setSize(CEGUI.UVector2(CEGUI.UDim(0.4, 0), CEGUI.UDim(0, P.textHeight)))
    title:setText(representation:getPickupName())
    title:setProperty("FrameEnabled", "set:False")
    item:addChildWindow(title)

    local useButton = winMgr:createWindow("MenuWidgets/Button", name .. "/UseButton")
    useButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, P.imageHeight+10),CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    useButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    if pickup:isUsed() == false then
        useButton:setText("use")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseButton_clicked")
    else
        useButton:setText("unuse")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUnuseButton_clicked")
    end
    item:addChildWindow(useButton)

    local dropButton = winMgr:createWindow("MenuWidgets/Button", name .. "/DropButton")
    dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, P.imageHeight+15+P.buttonWidth),CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    dropButton:setText("drop")
    orxonox.GUIManager:subscribeEventHelper(dropButton, "Clicked", P.name .. ".InventoryDropButton_clicked")
    item:addChildWindow(dropButton)

    local detailsButton = winMgr:createWindow("MenuWidgets/Button", name .. "/DetailsButton")
    detailsButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, P.imageHeight+20+2*P.buttonWidth),CEGUI.UDim(0, (P.imageHeight-P.textHeight)/2)))
    detailsButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    detailsButton:setText("details")
    orxonox.GUIManager:subscribeEventHelper(detailsButton, "Clicked", P.name .. ".InventoryDetailsButton_clicked")
    item:addChildWindow(detailsButton)

    return item
end

function P.cleanup()
    if P.wrapper ~= nil then
        winMgr:destroyWindow(P.wrapper)
    end
    
    --Destroy details windows.
    for k,v in pairs(P.detailsWindows) do
        if v ~= nil then
            winMgr:destroyWindow(v)
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
    local representation = orxonox.PickupManager:getInstance():getPickupRepresentation(pickup)
    
    local headerOffset = 35
    --Design parameters
    local imageSize = 100
    
    local name = "orxonox/PickupInventory/Pickup" .. pickupIndex .. "/Details" .. P.getNewDetailNumber()
    
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
    if pickup:isUsed() == false then
        useButton:setText("use")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseButton_clicked")
    else
        useButton:setText("unuse")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUnuseButton_clicked")
    end
    wrapper:addChildWindow(useButton)
    
    local dropButton = winMgr:createWindow("MenuWidgets/Button", name .. "/DropButton")
    dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.detailImageSize+10+P.buttonWidth+10),CEGUI.UDim(1, -40)))
    dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.buttonWidth), CEGUI.UDim(0, P.textHeight)))
    dropButton:setText("drop")
    orxonox.GUIManager:subscribeEventHelper(dropButton, "Clicked", P.name .. ".InventoryDropButton_clicked")
    wrapper:addChildWindow(dropButton)
    
    table.insert(P.detailsWindows, window)
    
end

function P.getNewDetailNumber()
    local number = table.getn(P.detailsWindows)
    for k,v in pairs(P.detailsWindows) do
        if v == nil then
            number = k-1
        end
    end
    return number
end

function P.InventoryUseButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    orxonox.PickupManager:getInstance():usePickup(P.pickupsList[pickupIndex], true)
end

function P.InventoryUnuseButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    orxonox.PickupManager:getInstance():usePickup(P.pickupsList[pickupIndex], false)
end

function P.InventoryDropButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    orxonox.PickupManager:getInstance():dropPickup(P.pickupsList[pickupIndex])
end

function P.InventoryDetailsButton_clicked(e)
    local pickupIndex = P.windowToPickupHelper(e)
    P.createDetailsWindow(pickupIndex)
end

function P.closeDetailWindow(e)
    --Get some numbers from the window
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    local pickupNr = tonumber(match())
    local detailNr = tonumber(match())
    
    local window = P.detailsWindows[detailNr+1]
    winMgr:destroyWindow(window)
    P.detailsWindows[detailNr+1] = nil
end

function P.InventoryBackButton_clicked(e)
    orxonox.CommandExecutor:execute("OrxonoxOverlay toggleVisibility PickupInventory")
end

return P
