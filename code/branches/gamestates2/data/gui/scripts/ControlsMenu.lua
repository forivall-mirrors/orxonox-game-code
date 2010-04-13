-- ControlsMenu.lua

local P = createMenuSheet("ControlsMenu")

function P.ControlsMouseControlsButton_clicked(e)
    showMenuSheet("MouseControlsMenu", true)
end

function P.ControlsKeyboardControlsButton_clicked(e)
    showMenuSheet("KeyBindMenu", true)
end

function P.ControlsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

