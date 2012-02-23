-- SingleplayerMenu.lua

local P = createMenuSheet("SingleplayerConfigMenu")

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
    P.sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/SingleplayerConfigMenu/MiscConfigPane/SampleWindow")
    P.sampleWindow:setText("SampleText")

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerConfigMenu/CancelButton"),
            ["callback"]  = P.SingleplayerConfigCancelButton_clicked
    })
    
    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/SingleplayerConfigMenu/OKButton"),
            ["callback"]  = P.SingleplayerConfigOKButton_clicked
    })
end

function P.loadConfig(level)
    P.commandList = {}
    table.insert(P.commandList, "Gametype initialStartCountdown_")
    table.insert(P.commandList, "Gametype bAutoStart_")
    table.insert(P.commandList, "Gametype numberOfBots_")
    table.insert(P.commandList, "Dynamicmatch gameTime_")
    table.insert(P.commandList, "Dynamicmatch friendlyfire")
    table.insert(P.commandList, "Dynamicmatch tutorial")
    table.insert(P.commandList, "LastManStanding lives")
    table.insert(P.commandList, "LastManStanding respawnDelay")
    table.insert(P.commandList, "LastManStanding bNoPunishment")
    table.insert(P.commandList, "LastManStanding bHardPunishment")
    table.insert(P.commandList, "LastTeamStanding lives")
    table.insert(P.commandList, "LastTeamStanding respawnDelay")
    table.insert(P.commandList, "LastTeamStanding bNoPunishment")
    table.insert(P.commandList, "LastTeamStanding bHardPunishment")
    table.insert(P.commandList, "TeamDeathmatch teams_")
    table.insert(P.commandList, "UnderAttack gameTime_")

    P.nameList = {}
    table.insert(P.nameList, "Start countdown")
    table.insert(P.nameList, "Autostart")
    table.insert(P.nameList, "Number of Bots")
    table.insert(P.nameList, "Dynamicmatch: game time")
    table.insert(P.nameList, "Dynamicmatch: friendly fire")
    table.insert(P.nameList, "Dynamicmatch: tutorial")
    table.insert(P.nameList, "LastManStanding: lives")
    table.insert(P.nameList, "LastManStanding: respawn delay")
    table.insert(P.nameList, "LastManStanding: no punishment")
    table.insert(P.nameList, "LastManStanding: hard punishment")
    table.insert(P.nameList, "LastTeamStanding: lives")
    table.insert(P.nameList, "LastTeamStanding: respawn delay")
    table.insert(P.nameList, "LastTeamStanding: no punishment")
    table.insert(P.nameList, "LastTeamStanding: hard punishment")
    table.insert(P.nameList, "TeamDeathmatch: Number of teams")
    table.insert(P.nameList, "UnderAttack: game time")

    P.linesList = {}
    
    --Calculate design parameters:
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
    
    local pane = tolua.cast(winMgr:getWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane"), "CEGUI::ScrollablePane")
    size = pane:getViewableArea()
    P.editboxWidth = size:getWidth() - P.commandWidth - P.resetWidth - 5*P.spaceWidth

    P.createLines()
end

function P.createLine(k)
    local offset = 0
    -- destroy config line, if it already exists (otherwise would cause an error)
    if winMgr:isWindowPresent("orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k) then
        winMgr:destroyWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k)
    end
    -- content window for the entire line
    local line = winMgr:createWindow("DefaultWindow", "orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k)
    line:setHeight(CEGUI.UDim(0, P.lineHeight))
    line:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, P.lineHeight*(k-1))))

    -- config name
    local command = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Command")
    command:setText(P.nameList[k])
    command:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.commandWidth), CEGUI.UDim(1, 0)))
    command:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0, 0)))
    line:addChildWindow(command)
    offset = offset + P.commandWidth + P.spaceWidth

    -- config value (editable)
    local configvalue = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Configvalue")
    configvalue:setProperty("ReadOnly", "set:False")
    local value = orxonox.CommandExecutor:query("getConfig " .. P.commandList[k])
    configvalue:setText(value)
    configvalue:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.editboxWidth), CEGUI.UDim(0.9, 0)))
    configvalue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    -- enable the reset button if the value changed
    orxonox.GUIManager:subscribeEventHelper(configvalue, "TextAccepted", P.name .. ".SingleplayerConfigEditbox_textAccepted")
    line:addChildWindow(configvalue)
    offset = offset + P.editboxWidth + P.spaceWidth

    -- reset button (only available when value changed)
    local reset = winMgr:createWindow("MenuWidgets/Button", "orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Reset")
    reset:setSize(CEGUI.UVector2(CEGUI.UDim(0, P.resetWidth), CEGUI.UDim(0.9, 0)))
    reset:setPosition(CEGUI.UVector2(CEGUI.UDim(0, offset), CEGUI.UDim(0.05, 0)))
    reset:setText("reset")
    orxonox.GUIManager:subscribeEventHelper(reset, "Clicked", P.name .. ".SingleplayerConfigResetButton_clicked")
    line:addChildWindow(reset)
    reset:setEnabled(false)
    offset = offset + P.resetWidth + P.spaceWidth

    line:setWidth(CEGUI.UDim(0, offset))

    return line
end

function P.createLines()
    local window = winMgr:getWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane")

    for k,v in pairs(P.commandList) do
        local line = P.createLine(k)
        table.insert(P.linesList, line)
        window:addChildWindow(line)
    end

    local pane = tolua.cast(window, "CEGUI::ScrollablePane")
    pane:setVerticalStepSize(getScrollingStepSize(window))
end

function P.SingleplayerConfigOKButton_clicked(e)
    for k,v in pairs(P.commandList) do
        -- save the changes
        local editbox = winMgr:getWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Configvalue")
        orxonox.CommandExecutor:execute("config " .. P.commandList[k] .. " " .. editbox:getText())
        local resetButton = winMgr:getWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. k .. "/Reset")
        resetButton:setEnabled(false)
    end
    
    hideMenuSheet("SingleplayerConfigMenu")
end

function P.SingleplayerConfigCancelButton_clicked(e)
    hideMenuSheet("SingleplayerConfigMenu")
end

function P.SingleplayerConfigEditbox_textAccepted(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    local resetButton = winMgr:getWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. commandNr .. "/Reset")
    resetButton:setEnabled(true)
end

function P.SingleplayerConfigResetButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "%d+")
    local commandNr = tonumber(match())

    -- reload the old value
    local editbox = winMgr:getWindow("orxonox/SingleplayerConfigMenu/MiscConfigPane/ConfigCommand" .. commandNr .. "/Configvalue")
    local value = orxonox.CommandExecutor:query("getConfig " .. P.commandList[commandNr])
    editbox:setText(value)
    
    we.window:setEnabled(false)
end

return P
