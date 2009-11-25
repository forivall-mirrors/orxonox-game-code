-- NewCreditsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewCreditsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewCreditsMenu"
P.layoutString = "NewCreditsMenu.layout"

function P.CreditsBackButton_clicked(e)
    hideGUI("NewCreditsMenu")
    debug("event: back")
end

return P

