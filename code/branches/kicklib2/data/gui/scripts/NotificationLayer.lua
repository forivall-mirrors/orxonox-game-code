-- NotificationLayer.lua

local P = createMenuSheet("NotificationLayer", true, TriBool.True, TriBool.True)

P.queueList = {}
P.editMode = false

P.sampleWindow = nil

-- Loads the queues from the NotificationManager and creates the sample window, that is used to measure the width various strings need.
function P.onLoad()
    orxonox.NotificationManager:getInstance():loadQueues()
    P.sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/SampleWindow")
end

-- Creates a queue in the GUI.
function P.createQueue(name, size)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    local queue = winMgr:createWindow("MenuWidgets/Listbox", "orxonox/NotificationLayer/Root/Queue/" .. name)
    queue:setProperty("BackgroundColor", "00FFFFFF") -- Set background to be fully transparent.
    root:addChildWindow(queue)

    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    queue:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, 0), CEGUI.UDim(0, P.queueHeightHelper(queue, size))))

    local queueTuple =
    {
        ["window"]  = queue,
        ["name"]    = name,
        ["edit"]    = nil,
        ["visible"] = false
    }

    P.queueList[name] = queueTuple -- name access
    P.setVisible(queueTuple, false) -- Set the queue to invisible as long as there are no notifications in it.
end

-- Removes a queue from the GUI.
function P.removeQueue(queueName)
    local queue = P.queueList[queueName]

    if queue ~= nil then
        winMgr:destroyWindow(queue.window)
    end
    P.queueList[queueName] = nil
end

-- Pushes an input notification to the input queue.
function P.pushNotification(queueName, notification)
    local queue = P.queueList[queueName]
    if queue == nil then
        return
    end
    item = CEGUI.createListboxTextItem(notification)
    local listbox = CEGUI.toListbox(queue.window)
    -- Add the item to the top of the listbox.
    if listbox:getItemCount() == 0 then
        listbox:addItem(item)
    else
        listbox:insertItem(item, listbox:getListboxItemFromIndex(0))
    end

    -- If the queue has been invisible, set it to visible.
    if queue.visible == false then
        P.setVisible(queue, true)
    end
end

-- Pops the least recently added notification from the queue.
function P.popNotification(queueName)
    local queue = P.queueList[queueName]
    if queue == nil then
        return
    end
    local listbox = CEGUI.toListbox(queue.window)
    -- Removes the item from the bottom of the listbox.
    listbox:removeItem(listbox:getListboxItemFromIndex(listbox:getItemCount()-1))

    -- Sets the queue to invisible if there are no more notifications in it.
    if listbox:getItemCount() == 0 then
        P.setVisible(queue, false)
    end
end

-- Removes a notification at a given index from the queue.
function P.removeNotification(queueName, index)
    local queue = P.queueList[queueName]
    if queue == nil then
        return
    end
    local listbox = CEGUI.toListbox(queue.window)
    -- Removes the item.
    listbox:removeItem(listbox:getListboxItemFromIndex(tonumber(index)))

    -- Sets the queue to invisible if there are no more notifications in it.
    if listbox:getItemCount() == 0 then
        P.setVisible(queue, false)
    end
end

-- Clears the queue. Removes all notifications from it.
function P.clearQueue(queueName)
    local queue = P.queueList[queueName]
    if queue == nil then
        return
    end
    local listbox = CEGUI.toListbox(queue.window)
    CEGUI.toListbox(queue.window):resetList()

    -- Sets the queue to invisible.
    P.setVisible(queue, false)
end

-- Sets the visibility of the queue.
function P.setVisible(queue, visible)
    if queue == nil then
        return
    end
    queue.window:setVisible(visible)
    queue.visible = visible
end

-- Enter the edit mode of the notification layer.
function P.enterEditMode()
    P.editMode = true

    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")

    --Add control frame window.
    local window = winMgr:createWindow("MenuWidgets/FrameWindow", "orxonox/NotificationLayer/Root/EditMode/ControlWindow")
    local frame = tolua.cast(window, "CEGUI::FrameWindow")
    frame:setCloseButtonEnabled(false)
    frame:setText("NotificationLayer Control Window")
    frame:setSize(CEGUI.UVector2(CEGUI.UDim(0.7, 0), CEGUI.UDim(0.2, 0)))
    root:addChildWindow(window)
    local pane = winMgr:createWindow("MenuWidgets/ScrollablePane", "orxonox/NotificationLayer/Root/EditMode/ControlWindow/ScrollingPane")
    pane:setSize(CEGUI.UVector2(CEGUI.UDim(1,-20), CEGUI.UDim(1,-30)))
    pane:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10), CEGUI.UDim(0, 26)))
    window:addChildWindow(pane)

    vertOffset = 0
    horzOffset = 0
    -- Line to be able to create a new queue.
    local newQueueTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/ControlWindow/NewQueueTitle")
    newQueueTitle:setText("Create a new NotificationQueue:")
    local size = getMinTextSize(newQueueTitle)
    local textHeight = size[1]
    newQueueTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    newQueueTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(newQueueTitle)
    horzOffset = horzOffset + size[2] + 5
    local newQueueName = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/ControlWindow/NewQueueName")
    newQueueName:setProperty("ReadOnly", "set:False")
    newQueueName:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    newQueueName:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2] + 5
    pane:addChildWindow(newQueueName)
    local create = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/ControlWindow/CreateNewQueue")
    create:setText("create")
    P.sampleWindow:setText("create")
    size = getMinTextSize(P.sampleWindow)
    create:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]+20), CEGUI.UDim(0, textHeight)))
    create:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    orxonox.GUIManager:subscribeEventHelper(create, "Clicked", P.name .. ".createNewQueue_clicked")
    pane:addChildWindow(create)
    horzOffset = horzOffset + size[2]+20 + 5
    vertOffset = vertOffset + textHeight + 5

    horzOffset = 0
    -- Button to leave the edit mode.
    local leave = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/ControlWindow/LeaveEditModeButton")
    leave:setText("leave Edit Mode")
    P.sampleWindow:setText("leave Edit Mode")
    size = getMinTextSize(P.sampleWindow)
    leave:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]+20), CEGUI.UDim(0, textHeight)))
    leave:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    orxonox.GUIManager:subscribeEventHelper(leave, "Clicked", P.name .. ".leaveEditMode_clicked")
    pane:addChildWindow(leave)
    horzOffset = horzOffset + size[2]+20 + 5
    vertOffset = vertOffset + textHeight + 5

    --Replace all queues with FrameWindows
    for k,v in pairs(P.queueList) do
        if v ~= nil then
            local queue = P.queueList[k]
            -- Remove the window that displays the queue from the root window such that it is no longer displayed.
            root:removeChildWindow(v.window)

            -- Create the frame window, with options to edit the queue, that is displayed instead of the queue.
            local window = P.createQueueEditFrame(v.name)
            window:setArea(v.window:getArea()) -- Set the frame window size and position to the same as the queue.

            v.edit = window
        end
    end
end

-- Helper function. Creates a frame for the input queue.
function P.createQueueEditFrame(queueName)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")

    window = winMgr:createWindow("MenuWidgets/FrameWindow", "orxonox/NotificationLayer/Root/EditMode/" .. queueName)
    local frame = tolua.cast(window, "CEGUI::FrameWindow")
    frame:setCloseButtonEnabled(true)
    orxonox.GUIManager:subscribeEventHelper(frame, "CloseClicked", P.name .. ".closeQueue_clicked")
    frame:setText("NotificationQueue \"" .. queueName .. "\"")
    root:addChildWindow(window)
    local pane = winMgr:createWindow("MenuWidgets/ScrollablePane", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/ScrollingPane")
    pane:setSize(CEGUI.UVector2(CEGUI.UDim(1,-20), CEGUI.UDim(1,-30)))
    pane:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10), CEGUI.UDim(0, 26)))
    window:addChildWindow(pane)

    local horzOffset = 0
    local vertOffset = 0

    -- The line that lets you edit the targets of the queue.
    local targetsTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/TargetsTitle")
    targetsTitle:setText("Targets:")
    local size = getMinTextSize(targetsTitle)
    local textHeight = size[1]
    targetsTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    targetsTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(targetsTitle)
    horzOffset = horzOffset + size[2] + 5
    local targets = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Targets")
    targets:setProperty("ReadOnly", "set:False")
    local targetsText = orxonox.NotificationManager:getInstance():getQueue(queueName):getTargets()
    targets:setText(targetsText)
    P.sampleWindow:setText(targetsText)
    size = getMinTextSize(P.sampleWindow)
    targets:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]*2+20), CEGUI.UDim(0, textHeight)))
    targets:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2]*2+20 + 5
    pane:addChildWindow(targets)
    local save = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Targets/Save")
    save:setText("save")
    P.sampleWindow:setText("save")
    size = getMinTextSize(P.sampleWindow)
    local saveTextWidth = size[2]+20
    save:setSize(CEGUI.UVector2(CEGUI.UDim(0, saveTextWidth), CEGUI.UDim(0, textHeight)))
    save:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    orxonox.GUIManager:subscribeEventHelper(save, "Clicked", P.name .. ".saveTargets_clicked")
    pane:addChildWindow(save)
    horzOffset = horzOffset + saveTextWidth
    vertOffset = vertOffset + textHeight + 5

    horzOffset = 0
    -- The line that lets you edit the size of the queue.
    local sizeTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/SizeTitle")
    sizeTitle:setText("Size:")
    size = getMinTextSize(sizeTitle)
    sizeTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    sizeTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(sizeTitle)
    horzOffset = horzOffset + size[2] + 5
    local queueSize = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Size")
    queueSize:setProperty("ReadOnly", "set:False")
    local maxSize = orxonox.NotificationManager:getInstance():getQueue(queueName):getMaxSize()
    queueSize:setText(maxSize)
    P.sampleWindow:setText(maxSize)
    size = getMinTextSize(P.sampleWindow)
    queueSize:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]*2+20), CEGUI.UDim(0, textHeight)))
    queueSize:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2]*2+20 + 5
    pane:addChildWindow(queueSize)
    save = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Size/Save")
    save:setText("save")
    P.sampleWindow:setText("save")
    size = getMinTextSize(P.sampleWindow)
    local saveTextWidth = size[2]+20
    save:setSize(CEGUI.UVector2(CEGUI.UDim(0, saveTextWidth), CEGUI.UDim(0, textHeight)))
    save:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    orxonox.GUIManager:subscribeEventHelper(save, "Clicked", P.name .. ".saveSize_clicked")
    pane:addChildWindow(save)
    horzOffset = horzOffset + saveTextWidth
    vertOffset = vertOffset + textHeight + 5

    horzOffset = 0
    -- The line that lets you edit the display time of the queue.
    local displayTimeTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/DisplayTimeTitle")
    displayTimeTitle:setText("Display time:")
    size = getMinTextSize(displayTimeTitle)
    displayTimeTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    displayTimeTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(displayTimeTitle)
    horzOffset = horzOffset + size[2] + 5
    local displayTime = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/DisplayTime")
    displayTime:setProperty("ReadOnly", "set:False")
    local time = orxonox.NotificationManager:getInstance():getQueue(queueName):getDisplayTime()
    displayTime:setText(time)
    P.sampleWindow:setText(time)
    size = getMinTextSize(P.sampleWindow)
    displayTime:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]*2+20), CEGUI.UDim(0, textHeight)))
    displayTime:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2]*2+20 + 5
    pane:addChildWindow(displayTime)
    save = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/DisplayTime/Save")
    save:setText("save")
    P.sampleWindow:setText("save")
    size = getMinTextSize(P.sampleWindow)
    local saveTextWidth = size[2]+20
    save:setSize(CEGUI.UVector2(CEGUI.UDim(0, saveTextWidth), CEGUI.UDim(0, textHeight)))
    save:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    orxonox.GUIManager:subscribeEventHelper(save, "Clicked", P.name .. ".saveDisplayTime_clicked")
    pane:addChildWindow(save)
    horzOffset = horzOffset + saveTextWidth
    vertOffset = vertOffset + textHeight + 5

    return window
end

-- Leave the edit mode.
function P.leaveEditMode()
    P.editMode = false

    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    --Replace all queues with FrameWindows
    for k,v in pairs(P.queueList) do
        if v ~= nil then
            -- Add the queue window to the root window to have it displayed again.
            root:addChildWindow(v.window)
            -- Set the size and position of the queue window to the size and position of the queue edit frame.
            v.window:setArea(v.edit:getArea())
            -- Destroy the edit frame.
            winMgr:destroyWindow(v.edit)
            v.edit = nil
        end
    end

    --Remove control window
    winMgr:destroyWindow(winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/ControlWindow"))
end

-- Is called after the sheet has been hidden.
function P.onQuit()
    -- If we leave the edit mode we show the sheet again.
    if P.editMode then
        P.leaveEditMode()
        showMenuSheet(P.name, false, true)
    end
end

-- If the button to save the targets of a queue has been clicked.
function P. saveTargets_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "EditMode/.*/Targets/Save")
    local nameStr = match()
    local queueName = string.sub(nameStr, 10, string.len(nameStr)-13)

    local window = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Targets")
    local save = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Targets/Save")
    local width = window:getWidth():asAbsolute(1)

    local queue = orxonox.NotificationManager:getInstance():getQueue(queueName)
    -- Set the new targets.
    queue:setTargets(window:getText())
    local targets = queue:getTargets()

    window:setText(targets)
    P.sampleWindow:setText(targets)
    local size = getMinTextSize(P.sampleWindow)
    -- Adjust the width of the targets field.
    window:setWidth(CEGUI.UDim(0, size[2]*2+20))
    -- Adjust the position of the save button after the targets field.
    save:setXPosition(CEGUI.UDim(0, save:getXPosition():asAbsolute(1)-width+window:getWidth():asAbsolute(1)))
end

-- If the button to save the size if a queue has been clicked.
function P. saveSize_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "EditMode/.*/Size/Save")
    local nameStr = match()
    local queueName = string.sub(nameStr, 10, string.len(nameStr)-10)

    local window = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Size")
    local save = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/Size/Save")
    local width = window:getWidth():asAbsolute(1)

    local queue = orxonox.NotificationManager:getInstance():getQueue(queueName)
    -- Set the new size.
    queue:setMaxSize(tonumber(window:getText()))
    local maxSize = queue:getMaxSize()

    window:setText(maxSize)
    P.sampleWindow:setText(maxSize)
    local size = getMinTextSize(P.sampleWindow)
    -- Adjust the width of the size field.
    window:setWidth(CEGUI.UDim(0, size[2]*2+20))
    -- Adjust the position of the save button after the size field.
    save:setXPosition(CEGUI.UDim(0, save:getXPosition():asAbsolute(1)-width+window:getWidth():asAbsolute(1)))
end

-- If the button to save the display time if a queue has been clicked.
function P. saveDisplayTime_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "EditMode/.*/DisplayTime/Save")
    local nameStr = match()
    local queueName = string.sub(nameStr, 10, string.len(nameStr)-17)

    local window = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/DisplayTime")
    local save = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/" .. queueName .. "/DisplayTime/Save")
    local width = window:getWidth():asAbsolute(1)

    local queue = orxonox.NotificationManager:getInstance():getQueue(queueName)
    -- Set the new display time.
    queue:setDisplayTime(tonumber(window:getText()))
    local time = queue:getDisplayTime()

    window:setText(time)
    P.sampleWindow:setText(time)
    local size = getMinTextSize(P.sampleWindow)
    -- Adjust the width of the display time field.
    window:setWidth(CEGUI.UDim(0, size[2]*2+20))
    -- Adjust the position of the save button after the display time field.
    save:setXPosition(CEGUI.UDim(0, save:getXPosition():asAbsolute(1)-width+window:getWidth():asAbsolute(1)))
end

-- if the button to create a new queue has been clicked.
function P.createNewQueue_clicked(e)
    local window = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/ControlWindow/NewQueueName")
    local name = window:getText()

    local queue = P.queueList[name]
    -- Test if a queue with that name already exists.
    if queue ~= nil then
        window:setText("Queue with that name already exists.")
        return
    end

    -- Creates the new queue.
    orxonox.NotificationManager:getInstance():createQueue(name)

    queue = P.queueList[name]
    if queue == nil then
        return
    end

    -- Create the frame that represents the queue in edit mode, since that's what we're in.
    local frame = P.createQueueEditFrame(name)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    -- Remove the queue window from the root window, since we're in edit mode.
    root:removeChildWindow(queue.window)
    -- Set the frame window size and position to that of the queue window.
    frame:setArea(queue.window:getArea())
    queue.edit = frame

    -- Reset the text to create a new queue.
    window:setText("")
end

-- If the button to leave the edit mode has been clicked.
function P.leaveEditMode_clicked(e)
    hideMenuSheet(P.name)
end

-- If the button to close the queue has been clicked.
function P.closeQueue_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "EditMode/.*")
    local nameStr = match()
    local queueName = string.sub(nameStr, 10, string.len(nameStr))

    -- Destroy the frame window,
    winMgr:destroyWindow(P.queueList[queueName].edit)
    P.queueList[queueName].edit = nil
    -- Destroy the queue.
    orxonox.NotificationManager:getInstance():getQueue(queueName):destroy()
end

-- Helper function. Returns height a queue needs to have to display 'size' items.
function P.queueHeightHelper(queue, size)
    local listbox = CEGUI.toListbox(queue)
    local item = CEGUI.createListboxTextItem("Text")
    listbox:addItem(item)
    local singleItemHeight = listbox:getTotalItemsHeight()
    local lookAndFeel = CEGUI.WidgetLookManager:getSingleton():getWidgetLook(queue:getLookNFeel())
    local formattedArea = lookAndFeel:getNamedArea("ItemRenderingArea"):getArea():getPixelRect(queue)
    local frameHeight = queue:getUnclippedOuterRect():getHeight() - formattedArea:getHeight()
    listbox:removeItem(item)
    return frameHeight + singleItemHeight*size
end

return P
