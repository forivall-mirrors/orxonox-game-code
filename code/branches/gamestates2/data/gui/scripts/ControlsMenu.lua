-- ControlsMenu.lua

local P = createMenuSheet("ControlsMenu")

function P.ControlsMouseControlsButton_clicked(e)
    showGUI("MouseControlsMenu", true)
end

function P.ControlsKeyboardControlsButton_clicked(e)
    showGUI("KeyBindMenu", true)
end

function P.ControlsBackButton_clicked(e)
    hideGUI(P.name)
end

return P

