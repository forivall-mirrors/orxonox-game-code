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

function P.MouseControlsMouseScrollbar_clicked(e)
    -- mouse sensitivity
    debug("event: mouse sensitivity")
end

function P.MouseControlsDefaultButton_clicked(e)
    -- default control mode
    debug("event: default control mode")
end

function P.MouseControls_______Button_clicked(e)
    -- .......
    debug("event: .......")
end

function P.MouseControlsBackButton_clicked(e)
    hideGUI("NewMouseControlsMenu")
    debug("event: back")
end

return P

