-- MiscConfigMenu.lua

local P = createMenuSheet("MiscConfigMenu", true, TriBool.True, TriBool.True)

P.commandList = {}
P.nameList = {}
P.linesList = {}

P.sampleWindow = nil

P.lineHeight = 0
P.commandWidth = 0
P.configWidth = 0
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

    P.sampleWindow:setText("configure")
    size = getMinTextSize(P.sampleWindow)
    P.configWidth = size[2]+20

    P.sampleWindow:setText("reset")
    size = getMinTextSize(P.sampleWindow)
    P.resetWidth = size[2]+20

    P.spaceWidth = math.floor(1/8*P.configWidth)

    P.createLines()

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigBackButton"),
            ["callback"]  = P.MiscConfigBackButton_clicked
    })
end

function P.createLine(k)
    local offset = 0
    local line = winMgr:createWindow("DefaultWindow", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k)
    line:setHeight(CEGUI.UDim(0, P.lineHeight))
    line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, P.lineHeight*(k-1))))

    local command = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Command")
    command:setText(P.nameList[k])
    command:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.commandWidth), CEGUI.UDim(1, 0)))
    command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0, 0)))
    line:addChildWindow(command)
    offset = offset + P.commandWidth + P.spaceWidth

    local configvalue = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Configvalue")
    configvalue:setProperty("ReadOnly", "set:False")
    local value = orxonox.CommandExecutor:query("getConfig " .. P.commandList[k])
    configvalue:setText(value)
    P.sampleWindow:setText(value)
    local size = getMinTextSize(P.sampleWindow)
    local configvalueWidth = 2*size[2]
    configvalue:setSize(CEGUI.UVector2(CEGUI.UDim(0, configvalueWidth), CEGUI.UDim(0.9, 0)))
    configvalue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    line:addChildWindow(configvalue)
    offset = offset + configvalueWidth + P.spaceWidth

    local config = winMgr:createWindow("MenuWidgets/Button", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Config")
    config:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.configWidth), CEGUI.UDim(0.9, 0)))
    config:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    config:setText("configure")
    orxonox.GUIManager:subscribeEventHelper(config, "Clicked", P.name .. ".MiscConfigConfigure_clicked")
    line:addChildWindow(config)
    offset = offset + P.configWidth + P.spaceWidth

    local reset = winMgr:createWindow("MenuWidgets/Button", "orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Reset")
    reset:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.resetWidth), CEGUI.UDim(0.9, 0)))
    reset:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    reset:setText("reset")
    orxonox.GUIManager:subscribeEventHelper(reset, "Clicked", P.name .. ".MiscConfigReset_clicked")
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

function P.MiscConfigReset_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    -- TODO: Implement reset.
end

function P.MiscConfigConfigure_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    local window = winMgr:getWindow("orxonox/MiscConfigMenu/MiscConfigPane/ConfigCommand" .. commandNr .. "/Configvalue")

    orxonox.CommandExecutor:execute("config " .. P.commandList[commandNr] .. " " .. window:getText())
    local value = orxonox.CommandExecutor:query("getConfig " .. P.commandList[commandNr])
    window:setText(value)
end

function P.MiscConfigBackButton_clicked(e)
    hideMenuSheet("MiscConfigMenu")
end

return P
