-- PickupInventory.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package

if _REQUIREDNAME == nil then
    PickupInventory = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "PickupInventory"
P.layoutString = "PickupInventory.layout"

P.carrierList = {}

function P.init()
    carrierList = {}
end

function P.show()
    P.window:show() -- TODO: Do this through parent...
    P.visible = true
    
    local pickupManager = orxonox.PickupManager:getInstance()
    local carrier = pickupManager:getPawn()
    
    local root = winMgr:getWindow("orxonox/PickupInventory/Inventory")
    
    P.carrierList = {}
    
    P.getCarrierList(carrier)    
    for k,v in pairs(P.carrierList) do
        local window = P.createCarrierBox(v,k)
        root:addChildWindow(window);
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
    local window = winMgr:createWindow("TaharezLook/StaticText", name .. "/Title")
    --TODO: Align text to the top
    window:setText(carrier:getCarrierName())
    
    local box = winMgr:createWindow("TaharezLook/ScrollablePane", name .. "/Box")
    box:setPosition(CEGUI.UVector2(CEGUI.UDim(0.05, 0), CEGUI.UDim(0, 35)))
    box:setSize(CEGUI.UVector2(CEGUI.UDim(0.9, 0), CEGUI.UDim(1, 0)))
    window:addChildWindow(box)
    
    --Design parameters:
    imageHeight = 50
    textHeight = 30
    
    local numPickups = orxonox.PickupManager:getInstance():getNumPickups(carrier)
    for i=0,numPickups-1,1 do
        local pickup = orxonox.PickupManager:getInstance():getPickupRepresentation(i, carrier)
        
        local item = winMgr:createWindow("TaharezLook/StaticText", name .. "/Box/Pickup" .. i)
        item:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, imageHeight)))
        item:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, (imageHeight+5)*i)))
        box:addChildWindow(item)
        
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
        item:addChildWindow(title)
        
        local useButton = winMgr:createWindow("TaharezLook/Button", name .. "/Box/Pickup" .. i .. "/UseButton")
        useButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.4, imageHeight+10),CEGUI.UDim(0, (imageHeight-textHeight)/2)))
        useButton:setSize(CEGUI.UVector2(CEGUI.UDim(0.2, 0), CEGUI.UDim(0, textHeight)))
        useButton:setText("use")
        orxonox.GUIManager:subscribeEventHelper(useButton, "Clicked", P.filename .. ".InventoryUseButton_clicked")
        item:addChildWindow(useButton)
        
        local dropButton = winMgr:createWindow("TaharezLook/Button", name .. "/Box/Pickup" .. i .. "/DropButton")
        dropButton:setPosition(CEGUI.UVector2(CEGUI.UDim(0.6, imageHeight+15),CEGUI.UDim(0, (imageHeight-textHeight)/2)))
        dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(0.2, 0), CEGUI.UDim(0, textHeight)))
        dropButton:setText("drop")
        orxonox.GUIManager:subscribeEventHelper(dropButton, "Clicked", P.filename .. ".InventoryDropButton_clicked")
        item:addChildWindow(dropButton)
    end
    
    return window
end

function P.windowToCarrierHelper(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    debug(0, "Name: " .. name)

    local match = string.gmatch(name, "%d+")
    local carrierNr = tonumber(match())
    local pickupNr = tonumber(match())

    local arguments = {}
    arguments[1] = carrierNr
    arguments[2] = pickupNr
    return arguments
end

function P.InventoryUseButton_clicked(e)
    local arguments = P.windowToCarrierHelper(e)
    orxonox.PickupManager:getInstance():usePickup(arguments[2], P.carrierList[arguments[1]], true)
end

function P.InventoryDropButton_clicked(e)
    local arguments = P.windowToCarrierHelper(e)
    orxonox.PickupManager:getInstance():dropPickup(arguments[2], P.carrierList[arguments[1]])
end

function P.InventoryShowDetails_clicked(e)

end

function P.InventoryBackButton_clicked(e)
    hideGUI("PickupInventory")
end

return P
