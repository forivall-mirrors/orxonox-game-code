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
    
end

function P.show()
    P.window:show() -- TODO: Do this through parent...
    P.visible = true
    
    table.insert(P.carrierList, 4)
    
    local pickupManager = orxonox.PickupManager:getInstance()
    local carrier = pickupManager:getPawn()
    P.getCarrierList(carrier)
    
    for k,v in pairs(P.carrierList) do
        local args = {}
        table.insert(args, v)
        table.insert(args, k)
        local window = P.createCarrierBox(args)
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

function P.createCarrierBox(args)
    local carrier = args[1]
    local index = args[2]
    
    local name = "orxonox/PickupInventory/Carrier" .. index
    local window = winMgr:createWindow("TaharezLook/StaticText", name .. "/Title")
    window:setText(carrier:getCarrierName())
    -- TODO: Does this exist?
    local height = window:getHeight()
    
    local box = winMgr:createWindow("TaharezLook/ScrollablePane", name .. "/Box")
    box:setPosition(CEGUI.UVector2(CEGUI.UDim(0.05, 0), CEGUI.UDim(0, height)))
    box:setWidth(CEGUI.UDim(0.9, 0))
    
    local numPickups = orxonox.PickupManager.getInstance():getNumPickups(carrier)
    for i=0,numPickups-1,1 do
        
    end
    
    return window
end

function P.InventoryUseButton_clicked(e)

end

function P.InventoryDropButton_clicked(e)

end

function P.InventoryShowDetails_clicked(e)

end

function P.InventoryBackButton_clicked(e)
    hideGUI("PickupInventory")
end

return P
