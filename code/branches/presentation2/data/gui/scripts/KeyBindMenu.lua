-- KeyBindMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    KeyBindMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "KeyBindMenu"
P.layoutString = "KeyBindMenu.layout"

function P:init()
    commandList = {}
    table.insert(commandList, "fire 0")
    table.insert(commandList, "fire 1 | unfire")
    table.insert(commandList, "onpress fire 2")
    table.insert(commandList, "scale 1 moveFrontBack")
    table.insert(commandList, "scale -1 moveFrontBack")
    table.insert(commandList, "boost")
    table.insert(commandList, "scale 1 moveRightLeft")
    table.insert(commandList, "scale -1 moveRightLeft")
    table.insert(commandList, "scale 1 moveUpDown")
    table.insert(commandList, "scale -1 moveUpDown")
    table.insert(commandList, "scale 1 rotateRoll")
    table.insert(commandList, "scale -1 rotateRoll")
    table.insert(commandList, "switchCamera")
    table.insert(commandList, "openConsole")
    table.insert(commandList, "OverlayGroup toggleVisibility Debug")
    table.insert(commandList, "OverlayGroup toggleVisibility Stats")
    table.insert(commandList, "mouseLook")
    table.insert(commandList, "pause")
    
    nameList = {}
    table.insert(nameList, "Primary Fire")
    table.insert(nameList, "Secondary Fire")
    table.insert(nameList, "Fire Rocket")
    table.insert(nameList, "Accelerate")
    table.insert(nameList, "Break")
    table.insert(nameList, "Boost")
    table.insert(nameList, "Move Right")
    table.insert(nameList, "Move Left")
    table.insert(nameList, "Move Up")
    table.insert(nameList, "Move Down")
    table.insert(nameList, "Roll Right")
    table.insert(nameList, "Roll Left")
    table.insert(nameList, "Switch Camera")
    table.insert(nameList, "Open Console")
    table.insert(nameList, "Show Debug")
    table.insert(nameList, "Show Stats")
    table.insert(nameList, "Look Around")
    table.insert(nameList, "Pause")
    
    linesList = {}

    lineHeight = 35
    buttonWidth = 170
    clearWidth = 20
    
    local window = winMgr:getWindow("orxonox/KeyBindPane")

    for k,v in pairs(commandList) do
        local line = P.createLine(k)
        table.insert(linesList, line)
        window:addChildWindow(line)
    end
    
    local funct = luaState:createLuaFunctor("KeyBindMenu.callback()")
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(funct)
end

function P.createLine(k)
    local line = winMgr:createWindow("DefaultWindow", "orxonox/KeyBindPane/Binding" .. k)
    line:setHeight(CEGUI.UDim(0, lineHeight))
    line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, lineHeight*(k-1))))
    
    local command = winMgr:createWindow("TaharezLook/StaticText", "orxonox/KeyBindPane/Binding" .. k .. "/Command")
    command:setSize(CEGUI.UVector2(CEGUI.UDim(0, 200), CEGUI.UDim(1, 0)))
    command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    command:setText(nameList[k])
    line:addChildWindow(command)
    
    local plus = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Plus")
    plus:setSize(CEGUI.UVector2(CEGUI.UDim(0, clearWidth), CEGUI.UDim(1, 0)))
    plus:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 200), CEGUI.UDim(0, 0)))
    plus:setText("+")
    orxonox.KeyBinderManager:getInstance():subscribeEventHelper(plus, "Clicked", P.filename .. ".KeyBindPlus_clicked")
    line:addChildWindow(plus)
    
    numButtons = orxonox.KeyBinderManager:getInstance():getCurrent():getNumberOfBindings(commandList[k]);
    for i=0,(numButtons-1) do
        local button = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Button" .. i)
        button:setSize(CEGUI.UVector2(CEGUI.UDim(0, buttonWidth), CEGUI.UDim(0.8, 0)))
        button:setPosition(CEGUI.UVector2(CEGUI.UDim(0, (i*(buttonWidth+clearWidth))+200+clearWidth), CEGUI.UDim(0.1, 0)))
        button:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[k],i))
        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(button, "Clicked", P.filename .. ".KeyBindButton_clicked")
        --button:subscribeScriptedEvent("EventClicked", P.filename .. ".KeyBindButton_clicked")
        line:addChildWindow(button)
        
        local clear = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Clear" .. i)
        clear:setSize(CEGUI.UVector2(CEGUI.UDim(0, clearWidth), CEGUI.UDim(0.8, 0)))
        clear:setPosition(CEGUI.UVector2(CEGUI.UDim(0, (i*(buttonWidth+clearWidth)+buttonWidth)+200+clearWidth), CEGUI.UDim(0.1, 0)))
        clear:setText("X")
        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(clear, "Clicked", P.filename .. ".KeyBindClear_clicked")
        line:addChildWindow(clear)
    end
    
    line:setWidth(CEGUI.UDim(0, 200+numButtons*(buttonWidth+clearWidth)+clearWidth))
    
    return line
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
   
    orxonox.KeyBinderManager:getInstance():unbind(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[commandNr], buttonNr))
    
    P.callback()
end

function P.keybind(arguments)
    local commandNr = arguments[1]
    local buttonNr = arguments[2]
    if buttonNr ~= nil then
        orxonox.KeyBinderManager:getInstance():unbind(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[commandNr], buttonNr))
    end
    orxonox.KeyBinderManager:getInstance():keybind(commandList[commandNr])
end

function P.callback()
    while table.getn(linesList) ~= 0 do
        if linesList[1] ~= nil then
            winMgr:destroyWindow(linesList[1]:getName())
        end
        table.remove(linesList, 1)
    end
    
    linesList = {}

    window = winMgr:getWindow("orxonox/KeyBindPane")
    for q,w in pairs(commandList) do
        local line = P.createLine(q)
        table.insert(linesList, line)
        window:addChildWindow(line)
    end
    
    if(InfoPopup ~= nil) then
        InfoPopup.close()
    end
end

function P.KeyBindBackButton_clicked(e)
    hideGUI("KeyBindMenu")
end

return P

