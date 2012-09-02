-- MouseControlsMenu.lua

local P = createMenuSheet("MouseControlsMenu")

function P.onLoad()
    block = false
    mousenormalscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MouseNormalScrollbar"),"CEGUI::Scrollbar")
    mousederivescrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MouseDeriveScrollbar"),"CEGUI::Scrollbar")
    normalwindow = tolua.cast(winMgr:getWindow("orxonox/MouseNormalButton"),"CEGUI::RadioButton")
    derivewindow = tolua.cast(winMgr:getWindow("orxonox/MouseDeriveButton"),"CEGUI::RadioButton")
    invertwindow = tolua.cast(winMgr:getWindow("orxonox/MouseInvertCheckbox"),"CEGUI::Checkbox")
    mousenormalscrollbar_active = false
    mousederivescrollbar_active = false
    derive_active = orxonox.getConfig("KeyBinder","bDeriveMouseInput_")
    invert_active = false
    mousenormalsensitivity = orxonox.getConfig("KeyBinder","mouseSensitivity_")
    mousederivesensitivity = orxonox.getConfig("KeyBinder","mouseSensitivityDerived_")
    mousenormalscrollbarwindow:setScrollPosition((math.log(14*mousenormalsensitivity-6))/(6*math.log(2)))
    mousederivescrollbarwindow:setScrollPosition((math.log(14*mousederivesensitivity-6))/(6*math.log(2)))
    if derive_active == "true" then
        normal_active = false
        derive_active = true
        derivewindow:setSelected(derive_active)
    else
        normal_active = true
        derive_active = false
        normalwindow:setSelected(normal_active)
    end
    if invert_active == "true" then
        invert_active = true
    else
        invert_active = false
    end

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/MouseControlsBackButton"),
            ["callback"]  = P.MouseControlsBackButton_clicked
    })
end

function P.MouseControlsMouseNormalScrollbar_changed(e)
    if mousenormalscrollbar_active == false then
        scrollposition = mousenormalscrollbarwindow:getScrollPosition()
        mousenormalsensitivity = (math.pow(64,scrollposition)+6)/14
        orxonox.config("KeyBinder", "mouseSensitivity_", mousenormalsensitivity)
    end
end

function P.MouseControlsMouseNormalScrollbar_started(e)
    mousenormalscrollbar_active = true
end

function P.MouseControlsMouseNormalScrollbar_ended(e)
    scrollposition = mousenormalscrollbarwindow:getScrollPosition()
    mousenormalsensitivity = (math.pow(64,scrollposition)+6)/14
    orxonox.config("KeyBinder", "mouseSensitivity_", mousenormalsensitivity)
    mousenormalscrollbar_active = false
end

function P.MouseControlsMouseDeriveScrollbar_changed(e)
    if mousederivescrollbar_active == false then
        scrollposition = mousederivescrollbarwindow:getScrollPosition()
        mousederivesensitivity = (math.pow(64,scrollposition)+6)/14
        orxonox.config("KeyBinder", "mouseSensitivityDerived_", mousederivesensitivity)
    end
end

function P.MouseControlsMouseDeriveScrollbar_started(e)
    mousederivescrollbar_active = true
end

function P.MouseControlsMouseDeriveScrollbar_ended(e)
    scrollposition = mousederivescrollbarwindow:getScrollPosition()
    mousederivesensitivity = (math.pow(64,scrollposition)+6)/14
    orxonox.config("KeyBinder", "mouseSensitivityDerived_", mousederivesensitivity)
    mousederivescrollbar_active = false
end

function P.MouseNormalButton_clicked(e)
    if block == false then
        block = true
        derivewindow:setSelected(false)
        block = false
        orxonox.config("KeyBinder", "bDeriveMouseInput_", 0)
    end
end

function P.MouseDeriveButton_clicked(e)
    if block == false then
        block = true
        normalwindow:setSelected(false)
        block = false
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

