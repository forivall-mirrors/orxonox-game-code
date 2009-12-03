-- NewMouseControlsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewMouseControlsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewMouseControlsMenu"
P.layoutString = "NewMouseControlsMenu.layout"

function P.KeyBindBackButton_clicked(e)
    hideGUI("NewMouseControlsMenu")
    debug("event: back")
end

return P

