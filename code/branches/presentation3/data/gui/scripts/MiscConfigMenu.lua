-- MiscConfigMenu.lua

local P = createMenuSheet("MiscConfigMenu")

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
    table.insert(P.commandList, "ChatOverlay displayTime_")

    P.nameList = {}
    table.insert(P.nameList, "ChatOverlay: display time")

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
    orxonox.CommandExecutor:execute("getConfig " .. P.commandList[k])
    local value = orxonox.CommandExecutor:getReturnValueString()
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

    -- TODO: tconfig or permanent?
    orxonox.CommandExecutor:execute("tconfig " .. P.commandList[commandNr] .. " " .. window:getText())
    orxonox.CommandExecutor:execute("getConfig " .. P.commandList[commandNr])
    local value = orxonox.CommandExecutor:getReturnValueString()
    window:setText(value)
end

function P.MiscConfigBackButton_clicked(e)
    hideMenuSheet("MiscConfigMenu")
end

return P
