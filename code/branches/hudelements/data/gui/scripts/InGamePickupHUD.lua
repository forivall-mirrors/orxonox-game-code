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
  -- wie kann man hier dynamisch neue ListBoxItems erstellen? 
  -- wie können sich diese ListBoxItems selbst aktualisieren? <<Tickable?>>
end


-- events for ingamemenu



function P.close_button_clicked()
    hideGUI("InGamePickupHUD")
end

return P

