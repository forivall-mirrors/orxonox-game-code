-- MouseControlsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    MouseControlsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "MouseControlsMenu"
P.layoutString = "MouseControlsMenu.layout"

local scrollbar_active = false

function P.MouseControlsMouseScrollbar_changed(e)
    if scrollbar_active == false then
        -- mouse sensitivity
        debug("event: mouse sensitivity")
    end
end

function P.MouseControlsMouseScrollbar_started(e)
    scrollbar_active = true
end

function P.MouseControlsMouseScrollbar_ended(e)
    -- mouse sensitivity
    debug("event: mouse sensitivity")
    scrollbar_active = false
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
    hideGUI(P.filename)
end

return P

