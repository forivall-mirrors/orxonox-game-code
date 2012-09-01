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

P.lastEquipmentCount_ = 0
P.lastUsableCount_ = 0
P.currentUsableID_ = 0

-- events
function P:frmUpdate(e)
    local equipCount = orxonox.PickupInventory:getEquipmentCount()
    local usableCount = orxonox.PickupInventory:getUsableCount()

    if equipCount ~= self.lastEquipmentCount_ or usableCount ~= self.lastUsableCount_ then
        self:updateTabs()
    end
end

function P.update(e)
    loadedGUIs["PickupInventory"]:frmUpdate(e)
end

function P.itemClicked(e)
    loadedGUIs["PickupInventory"]:mItemClicked(e)
end

function P:mItemClicked(e)
    local w = CEGUI.toWindowEventArgs(e).window
    local name = w:getName()
    local t = name:sub(25, 27)
    local i = name:sub(29)

    if t == "equ" then

    end

    if t == "use" then
        if self.currentUsableID_ >= 0 then
            winMgr:getWindow("orxonox/Inventory/Title/use/" .. self.currentUsableID_):setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF")
        end
        orxonox.PickupInventory:selectUsable(tonumber(i))
        self.currentUsableID_ = tonumber(i)
        winMgr:getWindow("orxonox/Inventory/Title/use/" .. i):setProperty("TextColours", "tl:FFFF4444 tr:FFFF4444 bl:FFFF4444 br:FFFF4444")
    end
end

-- methods
function P:updateTabs()
    local eqWin = winMgr:getWindow("orxonox/Inventory/TabControl/TabEquipment")
    local usWin = winMgr:getWindow("orxonox/Inventory/TabControl/TabUsable")
    orxonox.PickupInventory:getSingleton():clearInventory(winMgr, eqWin, usWin)
    orxonox.PickupInventory:getSingleton():updateTabs(winMgr, eqWin, usWin)

    self.currentUsableID_ = orxonox.PickupInventory:getCurrentUsableIndex()
    self.lastEquipmentCount_ = orxonox.PickupInventory:getEquipmentCount()
    self.lastUsableCount_ = orxonox.PickupInventory:getUsableCount()
end

return P
