-- ControlsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    ControlsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "ControlsMenu"
P.layoutString = "ControlsMenu.layout"

function P.ControlsMouseControlsButton_clicked(e)
    showGUI("MouseControlsMenu", true)
end

function P.ControlsKeyboardControlsButton_clicked(e)
    showGUI("KeyBindMenu", true)
end

function P.ControlsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

