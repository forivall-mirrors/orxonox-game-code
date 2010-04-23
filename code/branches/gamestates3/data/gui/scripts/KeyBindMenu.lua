-- KeyBindMenu.lua

local P = createMenuSheet("KeyBindMenu")
P.loadAlong = { "InfoPopup" }

function P.onLoad()

    P.commandList = {}
    table.insert(P.commandList, "fire 0")
    table.insert(P.commandList, "fire 1 | unfire")
    table.insert(P.commandList, "onpress fire 2")
    table.insert(P.commandList, "scale 1 moveFrontBack")
    table.insert(P.commandList, "scale -1 moveFrontBack")
    table.insert(P.commandList, "boost")
    table.insert(P.commandList, "scale 1 moveRightLeft")
    table.insert(P.commandList, "scale -1 moveRightLeft")
    table.insert(P.commandList, "scale 1 moveUpDown")
    table.insert(P.commandList, "scale -1 moveUpDown")
    table.insert(P.commandList, "scale -1 rotateRoll")
    table.insert(P.commandList, "scale 1 rotateRoll")
    table.insert(P.commandList, "scale 1 rotateYaw")
    table.insert(P.commandList, "scale -1 rotateYaw")
    table.insert(P.commandList, "scale 1 rotatePitch")
    table.insert(P.commandList, "scale -1 rotatePitch")
    table.insert(P.commandList, "NewHumanController changeMode")
    table.insert(P.commandList, "switchCamera")
    table.insert(P.commandList, "openConsole")
    table.insert(P.commandList, "OverlayGroup toggleVisibility Debug")
    table.insert(P.commandList, "OverlayGroup toggleVisibility Stats")
    table.insert(P.commandList, "mouseLook")
    table.insert(P.commandList, "pause")

    P.nameList = {}
    table.insert(P.nameList, "Primary Fire")
    table.insert(P.nameList, "Secondary Fire")
    table.insert(P.nameList, "Fire Rocket")
    table.insert(P.nameList, "Accelerate")
    table.insert(P.nameList, "Break")
    table.insert(P.nameList, "Boost")
    table.insert(P.nameList, "Move Right")
    table.insert(P.nameList, "Move Left")
    table.insert(P.nameList, "Move Up")
    table.insert(P.nameList, "Move Down")
    table.insert(P.nameList, "Roll Right")
    table.insert(P.nameList, "Roll Left")
    table.insert(P.nameList, "Yaw Left")
    table.insert(P.nameList, "Yaw Right")
    table.insert(P.nameList, "Pitch Up")
    table.insert(P.nameList, "Pitch Down")
    table.insert(P.nameList, "Switch Input Mode")
    table.insert(P.nameList, "Switch Camera")
    table.insert(P.nameList, "Open Console")
    table.insert(P.nameList, "Show Debug")
    table.insert(P.nameList, "Show Stats")
    table.insert(P.nameList, "Look Around")
    table.insert(P.nameList, "Pause")

    P.linesList = {}

    --Calculate design parameters:
    P.sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/KeyBindPane/SampleWindow")
    P.sampleWindow:setText("SampleText")

    local size = getMinTextSize(P.sampleWindow)
    P.lineHeight = size[1]

    P.commandWidth = 0
    for k,v in pairs(P.commandList) do
        P.sampleWindow:setText(P.nameList[k])
        size = getMinTextSize(P.sampleWindow)
        if size[2] > P.commandWidth then
            P.commandWidth = size[2]
        end
    end

    P.sampleWindow:setText("add")
    size = getMinTextSize(P.sampleWindow)
    P.addWidth = size[2]

    P.sampleWindow:setText("X")
    size = getMinTextSize(P.sampleWindow)
    P.clearWidth = size[2]

    P.spaceWidth = math.floor(1/14*P.commandWidth)

    P.buttonWidth = 145

    P.createLines()

    local funct = luaState:createLuaFunctor("KeyBindMenu.callback()")
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(funct)
end

function P.KeyNameNiceifier(key)
    local name = string.sub(key, string.find(key, '%.(.*)')+1)
    local group = string.sub(key, string.find(key, '(.*)%.'))
    group = string.sub(group,1,string.len(group)-1)
    if( group == "Keys") then
        return "Key " .. string.sub(name, string.find(name, 'Key(.*)')+3)
    elseif( group == "MouseButtons") then
        return "Mouse " .. name
    elseif( string.find(group, "JoyStickButtons") ~= nil ) then
        return "Joystick " .. name
    elseif( string.find(group, "JoyStickAxes") ~= nil ) then
        return "Joystick Axis" .. string.sub(name, 5, 6) .. string.sub(name, string.find(name, 'Axis%d%d(.*)')+6)
    elseif( group == "MouseAxes" ) then
        return "Mouse " .. string.sub(name, string.find(name, '.(.*)')+1) .. " " .. string.sub(name, 1, 1) .. "-Axis"
    end
    return key
end

function P.createLine(k)
    local offset = 0
    local line = winMgr:createWindow("DefaultWindow", "orxonox/KeyBindPane/Binding" .. k)
    line:setHeight(CEGUI.UDim(0, P.lineHeight))
    line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, P.lineHeight*(k-1))))

    local command = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/KeyBindPane/Binding" .. k .. "/Command")
    command:setText(P.nameList[k])
    command:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.commandWidth), CEGUI.UDim(1, 0)))
    command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0, 0)))
    line:addChildWindow(command)
    offset = offset + P.commandWidth + P.spaceWidth

    local plus = winMgr:createWindow("MenuWidgets/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Plus")
    plus:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.addWidth), CEGUI.UDim(0.7, 0)))
    plus:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.15, 0)))
    plus:setText("add")
    orxonox.GUIManager:subscribeEventHelper(plus, "Clicked", P.name .. ".KeyBindPlus_clicked")
    line:addChildWindow(plus)
    offset = offset + P.addWidth + P.spaceWidth

    local numButtons = orxonox.KeyBinderManager:getInstance():getCurrent():getNumberOfBindings(P.commandList[k]);
    for i=0,(numButtons-1) do
        local button = winMgr:createWindow("MenuWidgets/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Button" .. i)
        local name = orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(P.commandList[k],i)
        name = P.KeyNameNiceifier(name)
        button:setText(name)
        P.sampleWindow:setText(name)
        local size = getMinTextSize(P.sampleWindow)
        local buttonWidth = size[2]
        button:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0.7, 0)))
        button:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.15, 0)))
        orxonox.GUIManager:subscribeEventHelper(button, "Clicked", P.name .. ".KeyBindButton_clicked")
        --button:subscribeScriptedEvent("EventClicked", P.name .. ".KeyBindButton_clicked")
        line:addChildWindow(button)
        offset = offset + buttonWidth

        local clear = winMgr:createWindow("MenuWidgets/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Clear" .. i)
        clear:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.clearWidth), CEGUI.UDim(0.7, 0)))
        clear:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.15, 0)))
        clear:setText("X")
        orxonox.GUIManager:subscribeEventHelper(clear, "Clicked", P.name .. ".KeyBindClear_clicked")
        line:addChildWindow(clear)
        offset = offset + P.clearWidth + P.spaceWidth
    end

    line:setWidth(CEGUI.UDim(0, offset+P.clearWidth))

    return line
end

function P.createLines()
    local window = winMgr:getWindow("orxonox/KeyBindPane")

    for k,v in pairs(P.commandList) do
        local line = P.createLine(k)
        table.insert(P.linesList, line)
        window:addChildWindow(line)
    end

    local pane = tolua.cast(window, "CEGUI::ScrollablePane")
    pane:setVerticalStepSize(getScrollingStepSize(window))
end

function P.KeyBindButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())
    local buttonNr = tonumber(match())

    local arguments = {}
    arguments[1] = commandNr
    arguments[2] = buttonNr
    openInfoPopup("Press any button/key or move a mouse/joystick axis.", KeyBindMenu.keybind, false, arguments)
end

function P.KeyBindPlus_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    local arguments = {}
    arguments[1] = commandNr
    openInfoPopup("Press any button/key or move a mouse/joystick axis.", KeyBindMenu.keybind, false, arguments)
end

function P.KeyBindClear_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())
    local buttonNr = tonumber(match())

    local str = orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(P.commandList[commandNr], buttonNr)
    orxonox.KeyBinderManager:getInstance():unbind(str)

    P.callback()
end

function P.keybind(arguments)
    local commandNr = arguments[1]
    local buttonNr = arguments[2]
    if buttonNr ~= nil then
        local str = orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(P.commandList[commandNr], buttonNr)
        orxonox.KeyBinderManager:getInstance():unbind(str)
    end

    orxonox.KeyBinderManager:getInstance():keybind(P.commandList[commandNr])
end

function P.callback()
    local pane = tolua.cast(winMgr:getWindow("orxonox/KeyBindPane"), "CEGUI::ScrollablePane")
    local position = pane:getVerticalScrollPosition()
    while table.getn(P.linesList) ~= 0 do
        if P.linesList[1] ~= nil then
            winMgr:destroyWindow(P.linesList[1]:getName())
        end
        table.remove(P.linesList, 1)
    end

    P.linesList = {}

    P.createLines()
    if(InfoPopup ~= nil) then
        InfoPopup.close()
    end
    pane:setVerticalScrollPosition( position )
end

function P.KeyBindBackButton_clicked(e)
    hideMenuSheet("KeyBindMenu")
end

return P
