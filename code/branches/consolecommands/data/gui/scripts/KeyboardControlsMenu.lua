-- KeyboardControlsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    KeyboardControlsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "KeyboardControlsMenu"
P.layoutString = "KeyboardControlsMenu.layout"

function P.KeyboardControlsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P
