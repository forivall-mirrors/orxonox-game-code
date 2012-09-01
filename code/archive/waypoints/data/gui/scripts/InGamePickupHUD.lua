-- InGamePickupHUD.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    InGamePickupHUD = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "InGamePickupHUD"
P.layoutString = "InGamePickupHUD.layout"

function P:init()
end


-- events for ingamemenu


function P.button_Pickup_clicked(e)
    orxonox.Game:getInstance():popState()
    orxonox.Game:getInstance():popState()
    orxonox.Game:getInstance():requestState("mainmenu")
    hideGUI("InGameMenu")
end

function P.close_button_clicked()
    hideGUI("InGamePickupHUD")
end

return P

