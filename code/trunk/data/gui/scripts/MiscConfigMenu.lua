-- MiscConfigMenu.lua

local P = createMenuSheet("MiscConfigMenu", true, tribool(true), tribool(true))

P.commandList = {}
P.nameList = {}
P.linesList = {}

P.sampleWindow = nil

P.lineHeight = 0
P.commandWidth = 0
P.editboxWidth = 0
P.resetWidth = 0
P.spaceWidth = 0

function P.onLoad()

    P.commandList = {}
    table.insert(P.commandList, "KeyBinder mouseSensitivity_")
    table.insert(P.commandList, "KeyBinder mouseSensitivityDerived_")
    table.insert(P.commandList, "KeyBinder bDeriveMouseInput_")
    table.insert(P.commandList, "KeyBinder mouseWheelStepSize_")
    table.insert(P.commandList, "Shell maxHistoryLength_")
    table.insert(P.commandList, "Core bStartIOConsole_")
    table.insert(P.commandList, "Game fpsLimit_")
    table.insert(P.commandList, "Spectator speed_")
    table.insert(P.commandList, "SpaceShip bInvertYAxis_")
    table.insert(P.commandList, "LevelManager defaultLevelName_")
    table.insert(P.commandList, "Gametype initialStartCountdown_")
    table.insert(P.commandList, "Gametype bAutoStart_")
    table.insert(P.commandList, "Gametype numberOfBots_")
    table.insert(P.commandList, "UnderAttack gameTime_")
    table.insert(P.commandList, "TeamDeathmatch teams_")
    table.insert(P.commandList, "HumanPlayer nick_")
    table.insert(P.commandList, "ChatOverlay displayTime_")
    table.insert(P.commandList, "Core bDevMode_")
    table.insert(P.commandList, "HUDNavigation MarkerLimit_")
    table.insert(P.commandList, "HUDNavigation showDistance")

    P.nameList = {}
    table.insert(P.nameList, "Mouse sensitivity")
    table.insert(P.nameList, "Mouse acceleration")
    table.insert(P.nameList, "Derive mouse input")
    table.insert(P.nameList, "Mouse wheel stepsize")
    table.insert(P.nameList, "Shell: max. History length")
    table.insert(P.nameList, "Start IOConsole")
    table.insert(P.nameList, "FPS limit")
    table.insert(P.nameList, "Spectator speed")
    table.insert(P.nameList, "Invert Y-axis")
    table.insert(P.nameList, "Default level")
    table.insert(P.nameList, "Start countdown")
    table.insert(P.nameList, "Autostart")
    table.insert(P.nameList, "Number of Bots")
    table.insert(P.nameList, "UnderAttack: game time")
    table.insert(P.nameList, "TeamDeathmatch: Number of teams")
    table.insert(P.nameList, "Playername")
    table.insert(P.nameList, "Chat: display time")
    table.insert(P.nameList, "Developer's Mode")
    table.insert(P.nameList, "Marker Limit")
    table.insert(P.nameList, "Show Distance next to cursor")

    P.linesList = {}

    --Calculate design parameters:
    P.sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/MiscConfigMenu/MiscConfigPane/SampleWindow")
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

    P.sampleWindow:setText("reset")
    size = getMinTextSize(P.sampleWindow)
    P.resetWidth = size[2]+20

    P.spaceWidth = 10
    
    local pane = tolua.cast(winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane"), "CEGUI::ScrollablePane")
    size = pane:getViewableArea()
    P.editboxWidth = size:getWidth() - P.commandWidth - P.resetWidth - 5*P.spaceWidth

    P.createLines()

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/MiscConfigMenu/CancelButton"),
            ["callback"]  = P.MiscConfigCancelButton_clicked
    })
    
    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/MiscConfigMenu/OKButton"),
            ["callback"]  = P.MiscConfigOKButton_clicked
    })
end

function P.createLine(k)
    local offset = 0
    -- content window for the entire line
    local line = winMgr:createWindow("DefaultWindow", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k)
    line:setHeight(CEGUI.UDim(0, P.lineHeight))
    line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, P.lineHeight*(k-1))))

    -- config name
    local command = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Command")
    command:setText(P.nameList[k])
    command:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.commandWidth), CEGUI.UDim(1, 0)))
    command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0, 0)))
    line:addChildWindow(command)
    offset = offset + P.commandWidth + P.spaceWidth

    -- config value (editable)
    local configvalue = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Configvalue")
    configvalue:setProperty("ReadOnly", "set:False")
    local value = orxonox.CommandExecutor:query("getConfig " .. P.commandList[k])
    configvalue:setText(value)
    configvalue:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.editboxWidth), CEGUI.UDim(0.9, 0)))
    configvalue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    -- enable the reset button if the value changed
    orxonox.GUIManager:subscribeEventHelper(configvalue, "TextAccepted", P.name .. ".MiscConfigEditbox_textAccepted")
    line:addChildWindow(configvalue)
    offset = offset + P.editboxWidth + P.spaceWidth

    -- reset button (only available when value changed)
    local reset = winMgr:createWindow("MenuWidgets/Button", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Reset")
    reset:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.resetWidth), CEGUI.UDim(0.9, 0)))
    reset:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    reset:setText("reset")
    orxonox.GUIManager:subscribeEventHelper(reset, "Clicked", P.name .. ".MiscConfigResetButton_clicked")
    line:addChildWindow(reset)
    reset:setEnabled(false)
    offset = offset + P.resetWidth + P.spaceWidth

    line:setWidth(CEGUI.UDim(0, offset))

    return line
end

function P.createLines()
    local window = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane")

    for k,v in pairs(P.commandList) do
        local line = P.createLine(k)
        table.insert(P.linesList, line)
        window:addChildWindow(line)
    end

    local pane = tolua.cast(window, "CEGUI::ScrollablePane")
    pane:setVerticalStepSize(getScrollingStepSize(window))
end

function P.MiscConfigOKButton_clicked(e)
    for k,v in pairs(P.commandList) do
        -- save the changes
        local editbox = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Configvalue")
        orxonox.CommandExecutor:execute("config " .. P.commandList[k] .. " " .. editbox:getText())
        local resetButton = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Reset")
        resetButton:setEnabled(false)
    end
    
    hideMenuSheet("MiscConfigMenu")
end

function P.MiscConfigCancelButton_clicked(e)
    hideMenuSheet("MiscConfigMenu")
end

function P.MiscConfigEditbox_textAccepted(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    local resetButton = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. commandNr .. "/Reset")
    resetButton:setEnabled(true)
end

function P.MiscConfigResetButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    -- reload the old value
    local editbox = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. commandNr .. "/Configvalue")
    local value = orxonox.CommandExecutor:query("getConfig " .. P.commandList[commandNr])
    editbox:setText(value)
    
    we.window:setEnabled(false)
end

return P
