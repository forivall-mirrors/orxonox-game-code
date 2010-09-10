-- NotificationLayer.lua

local P = createMenuSheet("NotificationLayer", true, TriBool.True, TriBool.True)

P.queueList = {}
P.editMode = false

P.sampleWindow = nil

function P.onLoad()
    orxonox.NotificationManager:getInstance():loadQueues()
    P.sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/SampleWindow")
end

function P.createQueue(name, size)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    local queue = winMgr:createWindow("MenuWidgets/Listbox", "orxonox/NotificationLayer/Root/Queue/" .. name)
    queue:setProperty("BackgroundColor", "00FFFFFF")
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
    P.setVisible(queueTuple, false)
end

function P.removeQueue(name)
    local queue = P.queueList[name]

    if queue ~= nil then
        winMgr:destroyWindow(queue.window)
    end
    P.queueList[name] = nil
end

function P.pushNotification(queueName, notification)
    local queue = P.queueList[queueName]
    if queue == nil then
        cout(0, "0Queue is nil! " .. queueName)
        return
    end
    item = CEGUI.createListboxTextItem(notification)
    local listbox = CEGUI.toListbox(queue.window)
    if listbox:getItemCount() == 0 then
        listbox:addItem(item)
    else
        listbox:insertItem(item, listbox:getListboxItemFromIndex(0))
    end

    if queue.visible == false then
        P.setVisible(queue, true)
    end
end

function P.popNotification(queueName)
    local queue = P.queueList[queueName]
    if queue == nil then
        cout(0, "1Queue is nil! " .. queueName)
        return
    end
    local listbox = CEGUI.toListbox(queue.window)
    listbox:removeItem(listbox:getListboxItemFromIndex(listbox:getItemCount()-1))

    if listbox:getItemCount() == 0 then
        P.setVisible(queue, false)
    end
end

function P.removeNotification(queueName, index)
    local queue = P.queueList[queueName]
    if queue == nil then
        cout(0, "2Queue is nil! " .. queueName)
        return
    end
    local listbox = CEGUI.toListbox(queue.window)
    listbox:removeItem(listbox:getListboxItemFromIndex(tonumber(index)))

    if listbox:getItemCount() == 0 then
        P.setVisible(queue, false)
    end
end

function P.clearQueue(name)
    local queue = P.queueList[name]
    if queue == nil then
        cout(0, "3Queue is nil! " .. name)
        return
    end
    local listbox = CEGUI.toListbox(queue.window)
    CEGUI.toListbox(queue.window):resetList()

    P.setVisible(queue, false)
end

function P.changeSize(name, size)
    local queue = P.queueList[name]
    if queue == nil then
        cout(0, "5Queue is nil! " .. name)
        return
    end
    queue.window:setHeight(CEGUI.UDim(0, P.queueHeightHelper(queue.window, size)))
end

function P.setVisible(queue, visible)
    if queue == nil then
        cout(0, "6Queue is nil! " .. queue.name)
        return
    end
    queue.window:setVisible(visible)
    queue.visible = visible
end

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
            root:removeChildWindow(queue.window)
            
            local window = P.createQueueEditFrame(queue.name)
            window:setArea(queue.window:getArea())
            
            queue.edit = window
        end
    end
end

function P.createQueueEditFrame(name)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    window = winMgr:createWindow("MenuWidgets/FrameWindow", "orxonox/NotificationLayer/Root/EditMode/" .. name)
    local frame = tolua.cast(window, "CEGUI::FrameWindow")
    frame:setCloseButtonEnabled(true)
    orxonox.GUIManager:subscribeEventHelper(frame, "CloseClicked", P.name .. ".closeQueue_clicked")
    frame:setText("NotificationQueue \"" .. name .. "\"")
    root:addChildWindow(window)
    local pane = winMgr:createWindow("MenuWidgets/ScrollablePane", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/ScrollingPane")
    pane:setSize(CEGUI.UVector2(CEGUI.UDim(1,-20), CEGUI.UDim(1,-30)))
    pane:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10), CEGUI.UDim(0, 26)))
    window:addChildWindow(pane)

    local horzOffset = 0
    local vertOffset = 0

    local targetsTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/TargetsTitle")
    targetsTitle:setText("Targets:")
    local size = getMinTextSize(targetsTitle)
    local textHeight = size[1]
    targetsTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    targetsTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(targetsTitle)
    horzOffset = horzOffset + size[2] + 5
    local targets = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/Targets")
    targets:setProperty("ReadOnly", "set:False")
    local targetsText = orxonox.NotificationManager:getInstance():getQueue(name):getTargets()
    targets:setText(targetsText)
    P.sampleWindow:setText(targetsText)
    size = getMinTextSize(P.sampleWindow)
    targets:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]*2+20), CEGUI.UDim(0, textHeight)))
    targets:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2]*2+20 + 5
    pane:addChildWindow(targets)
    local save = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/Targets/Save")
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
    local sizeTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/SizeTitle")
    sizeTitle:setText("Size:")
    size = getMinTextSize(sizeTitle)
    sizeTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    sizeTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(sizeTitle)
    horzOffset = horzOffset + size[2] + 5
    local queueSize = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/Size")
    queueSize:setProperty("ReadOnly", "set:False")
    local maxSize = orxonox.NotificationManager:getInstance():getQueue(name):getMaxSize()
    queueSize:setText(maxSize)
    P.sampleWindow:setText(maxSize)
    size = getMinTextSize(P.sampleWindow)
    queueSize:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]*2+20), CEGUI.UDim(0, textHeight)))
    queueSize:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2]*2+20 + 5
    pane:addChildWindow(queueSize)
    save = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/Size/Save")
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
    local displayTimeTitle = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/DisplayTimeTitle")
    displayTimeTitle:setText("Display time:")
    size = getMinTextSize(displayTimeTitle)
    displayTimeTitle:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]), CEGUI.UDim(0, textHeight)))
    displayTimeTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    pane:addChildWindow(displayTimeTitle)
    horzOffset = horzOffset + size[2] + 5
    local displayTime = winMgr:createWindow("MenuWidgets/Editbox", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/DisplayTime")
    displayTime:setProperty("ReadOnly", "set:False")
    local time = orxonox.NotificationManager:getInstance():getQueue(name):getDisplayTime()
    displayTime:setText(time)
    P.sampleWindow:setText(time)
    size = getMinTextSize(P.sampleWindow)
    displayTime:setSize(CEGUI.UVector2(CEGUI.UDim(0, size[2]*2+20), CEGUI.UDim(0, textHeight)))
    displayTime:setPosition(CEGUI.UVector2(CEGUI.UDim(0, horzOffset), CEGUI.UDim(0, vertOffset)))
    horzOffset = horzOffset + size[2]*2+20 + 5
    pane:addChildWindow(displayTime)
    save = winMgr:createWindow("MenuWidgets/Button", "orxonox/NotificationLayer/Root/EditMode/" .. name .. "/DisplayTime/Save")
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

function P.leaveEditMode()
    P.editMode = false

    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    --Replace all queues with FrameWindows
    for k,v in pairs(P.queueList) do
        if v ~= nil then
            root:addChildWindow(v.window)
            v.window:setArea(v.edit:getArea())
            winMgr:destroyWindow(v.edit)
            v.edit = nil
        end
    end

    --Remove control window
    winMgr:destroyWindow(winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/ControlWindow"))
end

function P.afterHide()
    if P.editMode then
        P.leaveEditMode()
        showMenuSheet(P.name, false, true)
    end
end

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
    queue:setTargets(window:getText())
    local targets = queue:getTargets()

    window:setText(targets)
    P.sampleWindow:setText(targets)
    local size = getMinTextSize(P.sampleWindow)
    window:setWidth(CEGUI.UDim(0, size[2]*2+20))
    save:setXPosition(CEGUI.UDim(0, save:getXPosition():asAbsolute(1)-width+window:getWidth():asAbsolute(1)))
end

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
    queue:setMaxSize(tonumber(window:getText()))
    local maxSize = queue:getMaxSize()

    window:setText(maxSize)
    P.sampleWindow:setText(maxSize)
    local size = getMinTextSize(P.sampleWindow)
    window:setWidth(CEGUI.UDim(0, size[2]*2+20))
    save:setXPosition(CEGUI.UDim(0, save:getXPosition():asAbsolute(1)-width+window:getWidth():asAbsolute(1)))
end

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
    queue:setDisplayTime(tonumber(window:getText()))
    local time = queue:getDisplayTime()

    window:setText(time)
    P.sampleWindow:setText(time)
    local size = getMinTextSize(P.sampleWindow)
    window:setWidth(CEGUI.UDim(0, size[2]*2+20))
    save:setXPosition(CEGUI.UDim(0, save:getXPosition():asAbsolute(1)-width+window:getWidth():asAbsolute(1)))
end

function P.createNewQueue_clicked(e)
    local window = winMgr:getWindow("orxonox/NotificationLayer/Root/EditMode/ControlWindow/NewQueueName")
    local name = window:getText()
    orxonox.NotificationManager:getInstance():createQueue(name)

    local queue = P.queueList[name]
    if queue == nil then
        cout(0, "7Queue is nil! " .. name)
        return
    end
    
    local frame = P.createQueueEditFrame(name)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    root:removeChildWindow(queue.window)
    frame:setArea(queue.window:getArea())
    queue.edit = frame
    
    window:setText("")
end

function P.leaveEditMode_clicked(e)
    hideMenuSheet(P.name)
end

function P.closeQueue_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()

    local match = string.gmatch(name, "EditMode/.*")
    local nameStr = match()
    local queueName = string.sub(nameStr, 10, string.len(nameStr))

    winMgr:destroyWindow(P.queueList[queueName].edit)
    P.queueList[queueName].edit = nil
    orxonox.NotificationManager:getInstance():getQueue(queueName):destroy()
end

function P.queueHeightHelper(queue, size)
    local listbox = CEGUI.toListbox(queue)
    local item = CEGUI.createListboxTextItem("Text")
    listbox:addItem(item)
    local singleItemHeight = listbox:getTotalItemsHeight()
    local lookAndFeel = CEGUI.WidgetLookManager:getSingleton():getWidgetLook(queue:getLookNFeel())
    local formattedArea = lookAndFeel:getNamedArea("ItemRenderingArea"):getArea():getPixelRect(queue)
    local frameHeight = queue:getUnclippedPixelRect():getHeight() - formattedArea:getHeight()
    listbox:removeItem(item)
    return frameHeight + singleItemHeight*size
end

return P
