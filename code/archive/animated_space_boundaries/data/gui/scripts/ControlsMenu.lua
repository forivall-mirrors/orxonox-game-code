-- ControlsMenu.lua

local P = createMenuSheet("ControlsMenu")
P.loadAlong = { "MouseControlsMenu", "KeyBindMenu" }

function P.onLoad()
    P.multiplayerMode = "startClient"

    --buttons are arranged in a 3x1 matrix:
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/MouseControlsButton"),
            ["callback"]  = P.ControlsMouseControlsButton_clicked
    })

    P:setButton(2, 1, {
            ["button"] = winMgr:getWindow("orxonox/KeybindingsButton"),
            ["callback"]  = P.ControlsKeyboardControlsButton_clicked
    })

    P:setButton(3, 1, {
            ["button"] = winMgr:getWindow("orxonox/ControlsBackButton"),
            ["callback"]  = P.ControlsBackButton_clicked
    })
end

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

