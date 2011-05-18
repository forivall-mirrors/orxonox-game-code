-- KeyBindMenu.lua

local P = createMenuSheet("KeyBindMenu")
P.loadAlong = { "InfoPopup" }

function P.onLoad()

    commandList = {}
    table.insert(commandList, "fire 0")
    table.insert(commandList, "fire 1 | unfire")
    table.insert(commandList, "onpress fire 2")
    table.insert(commandList, "onpress fire 3")
    table.insert(commandList, "scale 1 moveFrontBack")
    table.insert(commandList, "scale -1 moveFrontBack")
    table.insert(commandList, "boost")
    table.insert(commandList, "scale 1 moveRightLeft")
    table.insert(commandList, "scale -1 moveRightLeft")
    table.insert(commandList, "scale 1 moveUpDown")
    table.insert(commandList, "scale -1 moveUpDown")
    table.insert(commandList, "scale -1 rotateRoll")
    table.insert(commandList, "scale 1 rotateRoll")
    table.insert(commandList, "scale 1 rotateYaw")
    table.insert(commandList, "scale -1 rotateYaw")
    table.insert(commandList, "scale 1 rotatePitch")
    table.insert(commandList, "scale -1 rotatePitch")
    table.insert(commandList, "NewHumanController changeMode")
    table.insert(commandList, "switchCamera")
    table.insert(commandList, "InGameConsole openConsole")
    table.insert(commandList, "OverlayGroup toggleVisibility Debug")
    table.insert(commandList, "OverlayGroup toggleVisibility Stats")
    table.insert(commandList, "OrxonoxOverlay toggleVisibility QuestGUI")
    table.insert(commandList, "OrxonoxOverlay toggleVisibility PickupInventory")
    table.insert(commandList, "startchat")
    table.insert(commandList, "startchat_small")
    table.insert(commandList, "mouseLook")
    table.insert(commandList, "pause")
    table.insert(commandList, "printScreen")
    if orxonox.GUIManager:inDevMode() then
        table.insert(commandList, "printScreenHD")
        table.insert(commandList, "createSkybox")
    end

    nameList = {}
    table.insert(nameList, "Primary Fire")
    table.insert(nameList, "Secondary Fire")
    table.insert(nameList, "Fire Rocket")
    table.insert(nameList, "Fire Alternative Rocket")
    table.insert(nameList, "Accelerate")
    table.insert(nameList, "Break")
    table.insert(nameList, "Boost")
    table.insert(nameList, "Move Right")
    table.insert(nameList, "Move Left")
    table.insert(nameList, "Move Up")
    table.insert(nameList, "Move Down")
    table.insert(nameList, "Roll Right")
    table.insert(nameList, "Roll Left")
    table.insert(nameList, "Yaw Left")
    table.insert(nameList, "Yaw Right")
    table.insert(nameList, "Pitch Up")
    table.insert(nameList, "Pitch Down")
    table.insert(nameList, "Switch Input Mode")
    table.insert(nameList, "Switch Camera")
    table.insert(nameList, "Open Console")
    table.insert(nameList, "Show Debug")
    table.insert(nameList, "Show Stats")
    table.insert(nameList, "Show Quests")
    table.insert(nameList, "Show Pickups")
    table.insert(nameList, "Show Chat")
    table.insert(nameList, "Show small Chat")
    table.insert(nameList, "Look Around")
    table.insert(nameList, "Pause")
    table.insert(nameList, "Screenshot")
    if orxonox.GUIManager:inDevMode() then
        table.insert(nameList, "HD screenshot")
        table.insert(nameList, "Create Skybox")
    end

    linesList = {}

    --Calculate design parameters:
    sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/KeyBindPane/SampleWindow")
    sampleWindow:setText("SampleText")

    local size = getMinTextSize(sampleWindow)
    lineHeight = size[1]

    commandWidth = 0
    for k,v in pairs(commandList) do
        sampleWindow:setText(nameList[k])
        size = getMinTextSize(sampleWindow)
        if size[2] > commandWidth then
            commandWidth = size[2]
        end
    end

    sampleWindow:setText("add")
    size = getMinTextSize(sampleWindow)
    addWidth = size[2]

    sampleWindow:setText("X")
    size = getMinTextSize(sampleWindow)
    clearWidth = size[2]

    spaceWidth = math.floor(1/14*commandWidth)

    buttonWidth = 145

    P.createLines()

    local funct = luaState:createLuaFunctor("KeyBindMenu.callback()")
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(funct)

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/KeyBindBackButton"),
            ["callback"]  = P.KeyBindBackButton_clicked
    })
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
        return "Joystick Axis " .. string.sub(name, 5, 6) .. string.sub(name, string.find(name, 'Axis%d%d(.*)')+6)
    elseif( group == "MouseAxes" ) then
        return "Mouse " .. string.sub(name, string.find(name, '.(.*)')+1) .. " " .. string.sub(name, 1, 1) .. "-Axis"
    end
    return key
end

function P.createLine(k)
    local offset = 0
    local line = winMgr:createWindow("DefaultWindow", "orxonox/KeyBindPane/Binding" .. k)
    line:setHeight(CEGUI.UDim(0, lineHeight))
    line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, lineHeight*(k-1))))

    local command = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/KeyBindPane/Binding" .. k .. "/Command")
    command:setText(nameList[k])
    command:setSize(CEGUI.UVector2(CEGUI.UDim(0, commandWidth), CEGUI.UDim(1, 0)))
    command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0, 0)))
    line:addChildWindow(command)
    offset = offset + commandWidth + spaceWidth

    local plus = winMgr:createWindow("MenuWidgets/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Plus")
    plus:setSize(CEGUI.UVector2(CEGUI.UDim(0, addWidth), CEGUI.UDim(0.7, 0)))
    plus:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.15, 0)))
    plus:setText("add")
    orxonox.GUIManager:subscribeEventHelper(plus, "Clicked", P.name .. ".KeyBindPlus_clicked")
    line:addChildWindow(plus)
    offset = offset + addWidth + spaceWidth

    local numButtons = orxonox.KeyBinderManager:getInstance():getCurrent():getNumberOfBindings(commandList[k]);
    for i=0,(numButtons-1) do
        local button = winMgr:createWindow("MenuWidgets/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Button" .. i)
        local name = orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[k],i)
        name = P.KeyNameNiceifier(name)
        button:setText(name)
        sampleWindow:setText(name)
        local size = getMinTextSize(sampleWindow)
        local buttonWidth = size[2]
        button:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0.7, 0)))
        button:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.15, 0)))
        orxonox.GUIManager:subscribeEventHelper(button, "Clicked", P.name .. ".KeyBindButton_clicked")
        --button:subscribeScriptedEvent("EventClicked", P.name .. ".KeyBindButton_clicked")
        line:addChildWindow(button)
        offset = offset + buttonWidth

        local clear = winMgr:createWindow("MenuWidgets/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Clear" .. i)
        clear:setSize(CEGUI.UVector2(CEGUI.UDim(0, clearWidth), CEGUI.UDim(0.7, 0)))
        clear:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.15, 0)))
        clear:setText("X")
        orxonox.GUIManager:subscribeEventHelper(clear, "Clicked", P.name .. ".KeyBindClear_clicked")
        line:addChildWindow(clear)
        offset = offset + clearWidth + spaceWidth
    end

    line:setWidth(CEGUI.UDim(0, offset+clearWidth))

    return line
end

function P.createLines()
    local window = winMgr:getWindow("orxonox/KeyBindPane")

    for k,v in pairs(commandList) do
        local line = P.createLine(k)
        table.insert(linesList, line)
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

    local str = orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[commandNr], buttonNr)
    orxonox.KeyBinderManager:getInstance():unbind(str)

    P.callback()
end

function P.keybind(arguments)
    local commandNr = arguments[1]
    local buttonNr = arguments[2]
    if buttonNr ~= nil then
        local str = orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[commandNr], buttonNr)
        orxonox.KeyBinderManager:getInstance():unbind(str)
    end

    orxonox.KeyBinderManager:getInstance():keybind(commandList[commandNr])
end

function P.callback()
    local pane = tolua.cast(winMgr:getWindow("orxonox/KeyBindPane"), "CEGUI::ScrollablePane")
    local position = pane:getVerticalScrollPosition()
    while table.getn(linesList) ~= 0 do
        if linesList[1] ~= nil then
            winMgr:destroyWindow(linesList[1]:getName())
        end
        table.remove(linesList, 1)
    end

    linesList = {}

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
