-- NewControlsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewControlsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewControlsMenu"
P.layoutString = "NewControlsMenu.layout"

function P.ControlsMouseControlsButton_clicked(e)
    showGUI("NewMouseControlsMenu")
    debug("event: MOUSE CONTROLS")
end

function P.ControlsKeyboardControlsButton_clicked(e)
    showGUI("NewKeyboardControlsMenu")
    debug("event: KEYBOARD CONTROLS")
end

function P.ControlsBackButton_clicked(e)
    hideGUI("NewControlsMenu")
    debug("event: back")
end

return P

