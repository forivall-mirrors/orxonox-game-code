-- ControlsMenu.lua

local P = createMenuSheet("ControlsMenu")
P.buttonList = {}
P.loadAlong = { "MouseControlsMenu", "KeyBindMenu" }

function P.onLoad()
    P.multiplayerMode = "startClient" 

    --buttons are arranged in a 3x1 matrix:
    local item = {
            ["button"] = winMgr:getWindow("orxonox/MouseControlsButton"),
            ["function"]  = P.ControlsMouseControlsButton_clicked
    }
    P.buttonList[1] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/KeybindingsButton"),
            ["function"]  = P.ControlsKeyboardControlsButton_clicked
    }
    P.buttonList[2] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/ControlsBackButton"),
            ["function"]  = P.ControlsBackButton_clicked
    }
    P.buttonList[3] = item

end

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
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

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 3, 1)
end

return P

