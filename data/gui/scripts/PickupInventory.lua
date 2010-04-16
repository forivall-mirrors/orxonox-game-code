-- PickupInventory.lua

local P = createMenuSheet("PickupInventory")

P.carrierList = {}
P.wrapper = nil
P.detailsWindows = {}

function P.onLoad()
    carrierList = {}
end

function P.onShow()
    P.createInventory()
end

function P.onHide()
    P.cleanup()
end

function P.update()
    P.cleanup()
    
    P.createInventory()
end

function P.createInventory()
    local pickupManager = orxonox.PickupManager:getInstance()
    local carrier = pickupManager:getPawn()
    
    local root = winMgr:getWindow("orxonox/PickupInventory/Inventory")
    P.wrapper = winMgr:createWindow("TaharezLook/ScrollablePane", "orxonox/PickupInventory/Inventory/Wrapper")
    P.wrapper:setSize(CEGUI.UVector2(CEGUI.UDim(1,0),CEGUI.UDim(1,0)))
    root:addChildWindow(P.wrapper)
    
    P.carrierList = {}
    
    --Design parameters:
    local space = 15
    
    P.getCarrierList(carrier)
    local offset = 0
    for k,v in pairs(P.carrierList) do
        local window = P.createCarrierBox(v,k)
        window:setYPosition(CEGUI.UDim(0,offset))
        offset = offset + window:getHeight():asAbsolute(1) + space
        P.wrapper:addChildWindow(window)
    end
end

function P.getCarrierList(carrier)

    -- TODO: Test for nil or 0?
    if carrier == nil then
        return
    end
    
    table.insert(P.carrierList, carrier)
    
    local numCarriers = orxonox.PickupManager:getInstance():getNumCarrierChildren(carrier)
    if numCarriers == 0 then
        return
    end
    
    for i=0,numCarriers-1,1 do
        local child = orxonox.PickupManager:getInstance():getCarrierChild(i, carrier)
        if child ~= nil then
            P.getCarrierList(child)
        end
    end
end

function P.createCarrierBox(carrier, index)

    local name = "orxonox/PickupInventory/Carrier" .. index
        
    --Design parameters:
    local imageHeight = 50
    local textHeight = 30
    local horizontalOffset = 20
    local buttonWidth = 85
    
    local offset = 0

    local box = winMgr:createWindow("TaharezLook/ScrollablePane", name .. "/Box")
    box:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horizontalOffset), CEGUI.UDim(0, 0)))
    box:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -horizontalOffset), CEGUI.UDim(1, 0)))
    
    offset = offset+textHeight
    local title = winMgr:createWindow("TaharezLook/StaticText", name .. "/Title")
    title:setText(carrier:getCarrierName())
    title:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, offset)))
    title:setProperty("FrameEnabled", "set:False")
    box:addChildWindow(title)
    
    local numPickups = orxonox.PickupManager:getInstance():getNumPickups(carrier)
    for i=0,numPickups-1,1 do
        local pickup = orxonox.PickupManager:getInstance():getPickupRepresentation(i, carrier)
        
        local item = winMgr:createWindow("TaharezLook/StaticText", name .. "/Box/Pickup" .. i)
        item:setSize(CEGUI.UVector2(CEGUI.UDim(1, -horizontalOffset), CEGUI.UDim(0, imageHeight)))
        item:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horizontalOffset), CEGUI.UDim(0, offset)))
        box:addChildWindow(item)
        offset = offset + imageHeight+5
        
        local image = winMgr:createWindow("TaharezLook/StaticImage", name .. "/Box/Pickup" .. i .. "/Image")
        image:setProperty("Image", "set:PickupInventory image:" .. pickup:getInventoryRepresentation())
        image:setProperty("BackgroundEnabled", "set:False")
        image:setProperty("FrameEnabled", "set:True")
        image:setSize(CEGUI.UVector2(CEGUI.UDim(0, imageHeight), CEGUI.UDim(0, imageHeight)))
        item:addChildWindow(image)
        
        local title = winMgr:createWindow("TaharezLook/StaticText", name .. "/Box/Pickup" .. i .. "/Title")
        title:setPosition(CEGUI.UVector2(CEGUI.UDim(0, imageHeight+5), CEGUI.UDim(0, (imageHeight-textHeight)/2)))
        title:setSize(CEGUI.UVector2(CEGUI.UDim(0.4, 0), CEGUI.UDim(0, textHeight)))
        title:setText(pickup:getPickupName())
        title:setProperty("FrameEnabled", "set:False")
        item:addChildWindow(title)
        
        local useButton = winMgr:createWindow("TaharezLook/Button", name .. "/Box/Pickup" .. i .. "/UseButton")
        useButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, imageHeight+10),CEGUI.UDim(0, (imageHeight-textHeight)/2)))
        useButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0, textHeight)))
        useButton:setText("use")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseButton_clicked")
        item:addChildWindow(useButton)
        
        local dropButton = winMgr:createWindow("TaharezLook/Button", name .. "/Box/Pickup" .. i .. "/DropButton")
        dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, imageHeight+15+buttonWidth),CEGUI.UDim(0, (imageHeight-textHeight)/2)))
        dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0, textHeight)))
        dropButton:setText("drop")
        orxonox.GUIManager:subscribeEventHelper(dropButton, "Clicked", P.name .. ".InventoryDropButton_clicked")
        item:addChildWindow(dropButton)
        
        local detailsButton = winMgr:createWindow("TaharezLook/Button", name .. "/Box/Pickup" .. i .. "/DetailsButton")
        detailsButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, imageHeight+20+2*buttonWidth),CEGUI.UDim(0, (imageHeight-textHeight)/2)))
        detailsButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0, textHeight)))
        detailsButton:setText("details")
        orxonox.GUIManager:subscribeEventHelper(detailsButton, "Clicked", P.name .. ".InventoryDetailsButton_clicked")
        item:addChildWindow(detailsButton)
    end
    
    box:setHeight(CEGUI.UDim(0,offset))
    
    return box
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

function P.windowToCarrierHelper(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local carrierNr = tonumber(match())
    local pickupNr = tonumber(match())

    local arguments = {}
    arguments[1] = carrierNr
    arguments[2] = pickupNr
    return arguments
end

function P.createDetailsWindow(pickupIndex, carrierIndex)
    local carrier = P.carrierList[carrierIndex]
    local pickup = orxonox.PickupManager:getInstance():getPickupRepresentation(pickupIndex, carrier)
    
    local headerOffset = 35
    --Design parameters
    local titleHeight = 30
    local imageSize = 100
    local buttonWidth = 85
    
    local name = "orxonox/PickupInventory/Carrier" .. carrierIndex .. "/Pickup" .. pickupIndex .. "/Details" .. P.getNewDetailNumber()
    
    local window = winMgr:createWindow("TaharezLook/FrameWindow", name)
    window:setSize(CEGUI.UVector2(CEGUI.UDim(0.5,0),CEGUI.UDim(0.4,0)))
    orxonox.GUIManager:subscribeEventHelper(window, "CloseClicked", P.name .. ".closeDetailWindow")
    
    local root = winMgr:getWindow("orxonox/PickupInventory/Background")
    root:addChildWindow(window)
    
    local wrapper = winMgr:createWindow("DefaultWindow", name .. "/Wrapper")
    wrapper:setSize(CEGUI.UVector2(CEGUI.UDim(1, -20),CEGUI.UDim(1, -50)))
    wrapper:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10),CEGUI.UDim(0, 40)))
    window:addChildWindow(wrapper)
    
    local title = winMgr:createWindow("TaharezLook/StaticText", name .. "/Title")
    title:setText(pickup:getPickupName())
    title:setHeight(CEGUI.UDim(0, titleHeight))
    title:setProperty("FrameEnabled", "set:False")
    title:setProperty("BackgroundEnabled", "set:False")
    wrapper:addChildWindow(title)
    
    local image = winMgr:createWindow("TaharezLook/StaticImage", name .. "/Image")
    image:setProperty("Image", "set:PickupInventory image:" .. pickup:getInventoryRepresentation())
    image:setProperty("BackgroundEnabled", "set:False")
    image:setProperty("FrameEnabled", "set:True")
    image:setSize(CEGUI.UVector2(CEGUI.UDim(0, imageSize), CEGUI.UDim(0, imageSize)))
    image:setYPosition(CEGUI.UDim(0, titleHeight + 5))
    wrapper:addChildWindow(image)
    
    local box = winMgr:createWindow("TaharezLook/ScrollablePane", name .. "/Description")
    box:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -1*(imageSize + 10)),CEGUI.UDim(1, -(titleHeight + 5 + titleHeight + 20))))
    box:setPosition(CEGUI.UVector2(CEGUI.UDim(0, imageSize + 10),CEGUI.UDim(0, titleHeight + 5)))
    local description = winMgr:createWindow("TaharezLook/StaticText", name .. "/Description/Text")
    description:setText(pickup:getPickupDescription())
    description:setProperty("HorzFormatting", "WordWrapLeftAligned")
    description:setProperty("VertFormatting", "TopAligned")
    box:addChildWindow(description)
    wrapper:addChildWindow(box)
    
    local useButton = winMgr:createWindow("TaharezLook/Button", name .. "/UseButton")
    useButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0, imageSize+10),CEGUI.UDim(1, -40)))
    useButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0, titleHeight)))
    useButton:setText("use")
    orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.name .. ".InventoryUseButton_clicked")
    wrapper:addChildWindow(useButton)
    
    local dropButton = winMgr:createWindow("TaharezLook/Button", name .. "/DropButton")
    dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0, imageSize+10+buttonWidth+10),CEGUI.UDim(1, -40)))
    dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0, titleHeight)))
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
    local arguments = P.windowToCarrierHelper(e)
    orxonox.PickupManager:getInstance():usePickup(arguments[2], P.carrierList[arguments[1]], true)
end

function P.InventoryDropButton_clicked(e)
    local arguments = P.windowToCarrierHelper(e)
    orxonox.PickupManager:getInstance():dropPickup(arguments[2], P.carrierList[arguments[1]])
end

function P.InventoryDetailsButton_clicked(e)
    local arguments = P.windowToCarrierHelper(e)
    P.createDetailsWindow(arguments[2], arguments[1])
end

function P.closeDetailWindow(e)
    --Get some numbers from the window
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    local carrierNr = tonumber(match())
    local pickupNr = tonumber(match())
    local detailNr = tonumber(match())
    
    local window = P.detailsWindows[detailNr+1]
    winMgr:destroyWindow(window)
    P.detailsWindows[detailNr+1] = nil
end

function P.InventoryBackButton_clicked(e)
    hideGUI("PickupInventory")
end

return P
