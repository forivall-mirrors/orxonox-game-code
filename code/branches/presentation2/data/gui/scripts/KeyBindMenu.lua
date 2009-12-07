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
    table.insert(commandList, "fire 1")
    table.insert(commandList, "fire 2")

    local lineHeight = 30
    
    local window = winMgr:getWindow("orxonox/KeyBindPane")

    for k,v in pairs(commandList) do
        local button = winMgr:createWindow("TaharezLook/Button", "orxonox/KeyBindPane/Binding" .. k .. "/Button")
        local command = winMgr:createWindow("TaharezLook/StaticText", "orxonox/KeyBindPane/Binding" .. k .. "/Command")
        local line = winMgr:createWindow("DefaultWindow", "orxonox/KeyBindPane/Binding" .. k)
        
        line:setSize(CEGUI.UVector2(CEGUI.UDim(1,-13),CEGUI.UDim(0, lineHeight)))
        button:setSize(CEGUI.UVector2(CEGUI.UDim(0.4, 0), CEGUI.UDim(1, 0)))
        command:setSize(CEGUI.UVector2(CEGUI.UDim(0.6, 0), CEGUI.UDim(1, 0)))
        
        button:setPosition(CEGUI.UVector2(CEGUI.UDim(0.6, 0),CEGUI.UDim(0, 0)))
        command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0),CEGUI.UDim(0, 0)))
        line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0),CEGUI.UDim(0, lineHeight*(k-1))))
        
        button:setText(orxonox.KeyBinderManager:getInstance():getCurrent():getBinding(v))
        command:setText(v)
        
        orxonox.KeyBinderManager:getInstance():subscribeEventHelper(button, "Clicked", P.filename .. ".KeyBindButton_clicked")
        --button:subscribeScriptedEvent("EventClicked", P.filename .. ".KeyBindButton_clicked")
        
        line:addChildWindow(command)
        line:addChildWindow(button)
        window:addChildWindow(line)
    end
end

function P.KeyBindButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    commandNr = tonumber(string.match(name, "%d+"))
    
    openInfoPopup("Press any button/key or move a mouse/joystick axis.", KeyBindMenu.keybind)

end

function P.keybind()
    orxonox.KeyBinderManager:getInstance():keybind(commandList[commandNr])
end

function P.KeyBindBackButton_clicked(e)
    hideGUI("KeyBindMenu")
    debug("event: back")
end

return P

