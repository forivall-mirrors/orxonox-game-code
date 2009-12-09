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

    local lineHeight = 30
    
    local window = winMgr:getWindow("orxonox/KeyBindPane")

    for k,v in pairs(commandList) do
        local line = winMgr:createWindow("DefaultWindow", "orxonox/KeyBindPane/Binding" .. k)
        local command = winMgr:createWindow("TaharezLook/StaticText", "orxonox/KeyBindPane/Binding" .. k .. "/Command")
        local button = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Button")
        local clear = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Clear")
        local button2 = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Button2")
        local clear2 = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Clear2")
        local add = winMgr:createWindow("TaharezLook/TabButton", "orxonox/KeyBindPane/Binding" .. k .. "/Add")
        
        line:setSize(CEGUI.UVector2(CEGUI.UDim(1, -13), CEGUI.UDim(0, lineHeight)))
        command:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(1, 0)))
        button:setSize(CEGUI.UVector2(CEGUI.UDim(0.25, 0), CEGUI.UDim(0.6, 0)))
        clear:setSize(CEGUI.UVector2(CEGUI.UDim(0.05, 0), CEGUI.UDim(0.6, 0)))
        button2:setSize(CEGUI.UVector2(CEGUI.UDim(0.25, 0), CEGUI.UDim(0.6, 0)))
        clear2:setSize(CEGUI.UVector2(CEGUI.UDim(0.05, 0), CEGUI.UDim(0.6, 0)))
        add:setSize(CEGUI.UVector2(CEGUI.UDim(0.05, 0), CEGUI.UDim(0.6, 0)))
        
        line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, lineHeight*(k-1))))        
        command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
        button:setPosition(CEGUI.UVector2(CEGUI.UDim(0.25, 0), CEGUI.UDim(0.2, 0)))
        clear:setPosition(CEGUI.UVector2(CEGUI.UDim(0.5, 0), CEGUI.UDim(0.2, 0)))
        button2:setPosition(CEGUI.UVector2(CEGUI.UDim(0.6, 0), CEGUI.UDim(0.2, 0)))
        clear2:setPosition(CEGUI.UVector2(CEGUI.UDim(0.85, 0), CEGUI.UDim(0.2, 0)))
        add:setPosition(CEGUI.UVector2(CEGUI.UDim(0.925, 0), CEGUI.UDim(0.2, 0)))
        
        command:setText(nameList[k])
        button:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(v))
        clear:setText("X")
        button2:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(v))
        clear2:setText("X")
        add:setText("+")

        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(button, "Clicked", P.filename .. ".KeyBindButton_clicked")
        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(clear, "Clicked", P.filename .. ".KeyBindClear_clicked")
        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(add, "Clicked", P.filename .. ".KeyBindAdd_clicked")
        --button:subscribeScriptedEvent("EventClicked", P.filename .. ".KeyBindButton_clicked")
        
        line:addChildWindow(command)
        line:addChildWindow(button)
        line:addChildWindow(clear)
        line:addChildWindow(button2)
        line:addChildWindow(clear2)
        line:addChildWindow(add)
        window:addChildWindow(line)
    end
end

function P.KeyBindButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    buttonNr = tonumber(string.match(name, "%d+"))
    openInfoPopup("Press any button/key or move a mouse/joystick axis.", KeyBindMenu.keybind)
end

function P.KeyBindClear_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    clearNr = tonumber(string.match(name, "%d+"))
end

function P.KeyBindAdd_clicked(e)
    
end

function P.keybind()
    local funct = luaState:createLuaFunctor("KeyBindMenu.callback(" .. buttonNr ..")")
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(funct)
    orxonox.KeyBinderManager:getInstance():keybind(commandList[buttonNr])
end

function P.callback(number)
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(nil)
    local button = winMgr:getWindow("orxonox/KeyBindPane/Binding" .. number .. "/Button")
    button:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[number]))
    
    InfoPopup.close()
end

function P.KeyBindBackButton_clicked(e)
    hideGUI("KeyBindMenu")
end

return P

