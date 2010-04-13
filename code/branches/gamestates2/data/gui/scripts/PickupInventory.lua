-- PickupInventory.lua

local P = createMenuSheet("PickupInventory")

P.lastEquipmentCount_ = 0
P.lastUsableCount_ = 0
P.currentUsableID_ = 0

-- events
function P.frmUpdate(e)
    local equipCount = orxonox.PickupInventory:getEquipmentCount()
    local usableCount = orxonox.PickupInventory:getUsableCount()

    if equipCount ~= P.lastEquipmentCount_ or usableCount ~= P.lastUsableCount_ then
        P.updateTabs()
    end
end

function P.update(e)
    loadedGUIs["PickupInventory"]:frmUpdate(e)
end

function P.itemClicked(e)
    loadedGUIs["PickupInventory"]:mItemClicked(e)
end

function P.mItemClicked(e)
    local w = CEGUI.toWindowEventArgs(e).window
    local name = w:getName()
    local t = name:sub(25, 27)
    local i = name:sub(29)

    if t == "equ" then

    end

    if t == "use" then
        if P.currentUsableID_ >= 0 then
            winMgr:getWindow("orxonox/Inventory/Title/use/" .. P.currentUsableID_):setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF")
        end
        orxonox.PickupInventory:selectUsable(tonumber(i))
        P.currentUsableID_ = tonumber(i)
        winMgr:getWindow("orxonox/Inventory/Title/use/" .. i):setProperty("TextColours", "tl:FFFF4444 tr:FFFF4444 bl:FFFF4444 br:FFFF4444")
    end
end

-- methods
function P.updateTabs()
    local eqWin = winMgr:getWindow("orxonox/Inventory/TabControl/TabEquipment")
    local usWin = winMgr:getWindow("orxonox/Inventory/TabControl/TabUsable")
    orxonox.PickupInventory:getSingleton():clearInventory(winMgr, eqWin, usWin)
    orxonox.PickupInventory:getSingleton():updateTabs(winMgr, eqWin, usWin)

    P.currentUsableID_ = orxonox.PickupInventory:getCurrentUsableIndex()
    P.lastEquipmentCount_ = orxonox.PickupInventory:getEquipmentCount()
    P.lastUsableCount_ = orxonox.PickupInventory:getUsableCount()
end

return P
