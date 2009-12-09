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
    table.insert(commandList, "scale 1 moveRightLeft")
    table.insert(commandList, "scale -1 moveRightLeft")
    table.insert(commandList, "scale 1 moveFrontBack")
    table.insert(commandList, "boost")
    table.insert(commandList, "scale -1 moveFrontBack")
    table.insert(commandList, "scale 1 rotateRoll")
    table.insert(commandList, "scale -1 rotateRoll")
    table.insert(commandList, "scale 1 moveUpDown")
    table.insert(commandList, "scale -1 moveUpDown")
    table.insert(commandList, "openConsole")
    table.insert(commandList, "OverlayGroup toggleVisibility Debug")
    table.insert(commandList, "OverlayGroup toggleVisibility Stats")
    table.insert(commandList, "mouseLook")
    table.insert(commandList, "pause")
    
    nameList = {}
    table.insert(nameList, "Primary Fire")
    table.insert(nameList, "Secondary Fire")
    table.insert(nameList, "Fire Rocket")
    table.insert(nameList, "Steer Right")
    table.insert(nameList, "Steer Left")
    table.insert(nameList, "Give Thrust")
    table.insert(nameList, "Boost")
    table.insert(nameList, "Hit Breaks")
    table.insert(nameList, "Roll Right")
    table.insert(nameList, "Roll Left")
    table.insert(nameList, "Up")
    table.insert(nameList, "Down")
    table.insert(nameList, "Open Console")
    table.insert(nameList, "Show Debug")
    table.insert(nameList, "Show Stats")
    table.insert(nameList, "mouseLook")
    table.insert(nameList, "Pause")

    local lineHeight = 30
    
    local window = winMgr:getWindow("orxonox/KeyBindPane")

    for k,v in pairs(commandList) do
        local line = winMgr:createWindow("DefaultWindow", "orxonox/KeyBindPane/Binding" .. k)
        local command = winMgr:createWindow("TaharezLook/StaticText", "orxonox/KeyBindPane/Binding" .. k .. "/Command")
        local button = winMgr:createWindow("TaharezLook/Button", "orxonox/KeyBindPane/Binding" .. k .. "/Button")
        local clear = winMgr:createWindow("TaharezLook/Button", "orxonox/KeyBindPane/Binding" .. k .. "/Clear")
        
        line:setSize(CEGUI.UVector2(CEGUI.UDim(1,-13),CEGUI.UDim(0, lineHeight)))
        command:setSize(CEGUI.UVector2(CEGUI.UDim(0.55, 0), CEGUI.UDim(1, 0)))
        button:setSize(CEGUI.UVector2(CEGUI.UDim(0.4, 0), CEGUI.UDim(1, 0)))
        clear:setSize(CEGUI.UVector2(CEGUI.UDim(0.05, 0),CEGUI.UDim(1,0)))
        
        line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0),CEGUI.UDim(0, lineHeight*(k-1))))        
        command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0),CEGUI.UDim(0, 0)))
        button:setPosition(CEGUI.UVector2(CEGUI.UDim(0.55, 0),CEGUI.UDim(0, 0)))
        clear:setPosition(CEGUI.UVector2(CEGUI.UDim(0.95, 0),CEGUI.UDim(0, 0)))
        
        command:setText(nameList[k])
        button:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(v))
        clear:setText("X")

        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(button, "Clicked", P.filename .. ".KeyBindButton_clicked")
        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(clear, "Clicked", P.filename .. ".KeyBindClear_clicked")
        --button:subscribeScriptedEvent("EventClicked", P.filename .. ".KeyBindButton_clicked")
        
        line:addChildWindow(command)
        line:addChildWindow(button)
        line:addChildWindow(clear)
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

function P.keybind()
    local funct = luaState:createLuaFunctor("KeyBindMenu.callback(" .. buttonNr ..")")
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(funct)
    orxonox.KeyBinderManager:getInstance():keybind(commandList[buttonNr])
end

function P.callback(number)
    orxonox.KeyBinderManager:getInstance():registerKeybindCallback(0)
    local button = winMgr:getWindow("orxonox/KeyBindPane/Binding" .. number .. "/Button")
    button:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(commandList[number]))
    
    InfoPopup.close()
end

function P.KeyBindBackButton_clicked(e)
    hideGUI("KeyBindMenu")
    debug("event: back")
end

return P

