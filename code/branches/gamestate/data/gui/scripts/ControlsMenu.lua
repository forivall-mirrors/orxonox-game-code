-- ControlsMenu.lua

local P = createSheet("ControlsMenu")

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

