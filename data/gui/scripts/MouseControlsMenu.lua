-- MouseControlsMenu.lua

local P = createMenuSheet("MouseControlsMenu")

function P.onLoad()
    P.block = false
    P.mousenormalscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MouseNormalScrollbar"),"CEGUI::Scrollbar")
    P.mousederivescrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MouseDeriveScrollbar"),"CEGUI::Scrollbar")
    P.normalwindow = tolua.cast(winMgr:getWindow("orxonox/MouseNormalButton"),"CEGUI::RadioButton")
    P.derivewindow = tolua.cast(winMgr:getWindow("orxonox/MouseDeriveButton"),"CEGUI::RadioButton")
    --P.invertwindow = tolua.cast(winMgr:getWindow("orxonox/MouseInvertCheckbox"),"CEGUI::Checkbox")
    P.mousenormalscrollbar_active = false
    P.mousederivescrollbar_active = false
    local derive_active = orxonox.getConfig("KeyBinder","bDeriveMouseInput_")
    local invert_active = false
    local normal_active
    P.mousenormalsensitivity = orxonox.getConfig("KeyBinder","mouseSensitivity_")
    P.mousederivesensitivity = orxonox.getConfig("KeyBinder","mouseSensitivityDerived_")
    P.mousenormalscrollbarwindow:setScrollPosition((math.log(14*P.mousenormalsensitivity-6))/(6*math.log(2)))
    P.mousederivescrollbarwindow:setScrollPosition((math.log(14*P.mousederivesensitivity-6))/(6*math.log(2)))
    if derive_active == "true" then
        normal_active = false
        derive_active = true
        P.derivewindow:setSelected(derive_active)
    else
        normal_active = true
        derive_active = false
        P.normalwindow:setSelected(normal_active)
    end
    if invert_active == "true" then
        invert_active = true
    else
        invert_active = false
    end
end

function P.MouseControlsMouseNormalScrollbar_changed(e)
    if P.mousenormalscrollbar_active == false then
        local scrollposition = P.mousenormalscrollbarwindow:getScrollPosition()
        P.mousenormalsensitivity = (math.pow(64,scrollposition)+6)/14
        orxonox.config("KeyBinder", "mouseSensitivity_", P.mousenormalsensitivity)
    end
end

function P.MouseControlsMouseNormalScrollbar_started(e)
    P.mousenormalscrollbar_active = true
end

function P.MouseControlsMouseNormalScrollbar_ended(e)
    local scrollposition = P.mousenormalscrollbarwindow:getScrollPosition()
    P.mousenormalsensitivity = (math.pow(64,scrollposition)+6)/14
    orxonox.config("KeyBinder", "mouseSensitivity_", P.mousenormalsensitivity)
    P.mousenormalscrollbar_active = false
end

function P.MouseControlsMouseDeriveScrollbar_changed(e)
    if P.mousederivescrollbar_active == false then
        local scrollposition = P.mousederivescrollbarwindow:getScrollPosition()
        P.mousederivesensitivity = (math.pow(64,scrollposition)+6)/14
        orxonox.config("KeyBinder", "mouseSensitivityDerived_", P.mousederivesensitivity)
    end
end

function P.MouseControlsMouseDeriveScrollbar_started(e)
    P.mousederivescrollbar_active = true
end

function P.MouseControlsMouseDeriveScrollbar_ended(e)
    local scrollposition = P.mousederivescrollbarwindow:getScrollPosition()
    P.mousederivesensitivity = (math.pow(64,scrollposition)+6)/14
    orxonox.config("KeyBinder", "mouseSensitivityDerived_", P.mousederivesensitivity)
    P.mousederivescrollbar_active = false
end

function P.MouseNormalButton_clicked(e)
    if P.block == false then
        P.block = true
        P.derivewindow:setSelected(false)
        P.block = false
        orxonox.config("KeyBinder", "bDeriveMouseInput_", 0)
    end
end

function P.MouseDeriveButton_clicked(e)
    if P.block == false then
        P.block = true
        P.normalwindow:setSelected(false)
        P.block = false
        orxonox.config("KeyBinder", "bDeriveMouseInput_", 1)
    end
end

function P.MouseInvertCheckbox_clicked(e)
    -- invert mouse
end

function P.MouseControlsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

