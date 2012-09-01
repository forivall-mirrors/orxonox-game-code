-- NotificationLayer.lua

local P = createMenuSheet("NotificationLayer", true, tribool(true), tribool(true))

P.queueList = {}

P.sampleWindow = nil

-- Loads the queues from the NotificationManager and creates the sample window, that is used to measure the width various strings need.
function P.onLoad()
    orxonox.NotificationManager:getInstance():loadQueues()
    P.sampleWindow = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/SampleWindow")
end

-- Creates a queue in the GUI.
function P.createQueue(name, size)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    --local queue = winMgr:createWindow("MenuWidgets/Listbox", "orxonox/NotificationLayer/Root/Queue/" .. name)
    --queue:setProperty("BackgroundColor", "00FFFFFF") -- Set background to be fully transparent.
    local queue = winMgr:createWindow("MenuWidgets/ScrollablePane", "orxonox/NotificationLayer/Root/Queue/" .. name)
    queue:setProperty("Alpha", 0.0)
    --queue:setProperty("FrameEnabled", "false")
    root:addChildWindow(queue)

    local queueTuple =
    {
        ["window"]    = queue,
        ["name"]      = name,
        ["maxSize"]   = size,
        ["visible"]   = false,
        ["fontSize"]  = 12,
        ["fontColor"] = "FFFFFFFF",
        ["alignment"] = "LeftAligned",
        ["items"]     = {},
        ["first"]     = 1,
        ["last"]      = 1
    }
    
    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    queue:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, 0), CEGUI.UDim(0, P.queueHeightHelper(queueTuple, size))))

    P.queueList[name] = queueTuple -- name access
    P.setVisible(queueTuple, false) -- Set the queue to invisible as long as there are no notifications in it.
end

-- Removes a queue from the GUI.
function P.removeQueue(queueName)
    local queue = P.queueList[queueName]

    if queue ~= nil then
        queue.window:getParent():removeChildWindow(queue.window)
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

    if not guiMgr:usingOldCEGUI() then
        notification = string.gsub(notification, "%[", "\\%[") -- escape '[' which is used to format text since cegui 0.7
    end

    local item = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/Queue/" .. queueName .. "/" .. queue.last)
    item:setText(notification)
    P.setItemFontHelper(item, queue, true)
    -- Add the item to the top of the queue.
    local itemHeight = P.itemHeightHelper(queue)
    if queue.last-queue.first > 0 then -- If the queue is not empty move all items down
        for i=queue.first,queue.last-1 do
            local item = queue.items[i]
            item:setYPosition(CEGUI.UDim(0, itemHeight*(queue.last-i)))
        end
    end
    queue.window:addChildWindow(item)
    item:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, itemHeight)))
    item:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    item:setProperty("Alpha", 1.0)
    item:setProperty("InheritsAlpha", "false")
    item:setProperty("BackgroundEnabled", "false")
    item:setProperty("FrameEnabled", "false")
    item:setProperty("HorzFormatting", queue.alignment)
    queue.items[queue.last] = item
    queue.last = queue.last+1

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
    local item = queue.items[queue.first]
    -- Removes the item from the bottom of the queue.
    queue.window:removeChildWindow(item)
    winMgr:destroyWindow(item)
    queue.first = queue.first+1

    -- Sets the queue to invisible if there are no more notifications in it.
    if queue.last-queue.first == 0 then
        P.setVisible(queue, false)
    end
end

-- Removes a notification at a given index from the queue. Where the 0th item is the newest and the nth the (n+1)th newest.
function P.removeNotification(queueName, index)
    local queue = P.queueList[queueName]
    if queue == nil then
        return
    end

    index = queue.last-tonumber(index)-1
    --if index == queue.first then -- If we want to remove the oldest notification, we can just use pop.
    --    P.popNotification(queueName)
    --    return
    --end

    -- Removes the item.
    local item = queue.items[index]
    queue.window:removeChildWindow(item)
    winMgr:destroyWindow(item)
    queue.items[index] = nil

    -- Move the items below, up.
    local itemHeight = P.itemHeightHelper(queue)
    local moved = false
    if index > queue.first then -- Move all older notifications up in the list.
        for i=index-1,-1,queue.first do
            orxout(i)
            item = queue.items[i]
            item:setYposition(CEGUI.UDim(0, itemHeight*(queue.last-i-1)))
            queue.items[i+1] = item
        end
    end
    queue.items[queue.first] = nil
    queue.first = queue.first+1

    -- Sets the queue to invisible if there are no more notifications in it.
    if queue.last-queue.first == 0 then
        P.setVisible(queue, false)
    end
end

-- Clears the queue. Removes all notifications from it.
function P.clearQueue(queueName)
    local queue = P.queueList[queueName]
    if queue == nil then
        return
    end
    for i=queue.first,queue.last-1 do
        local item = queue.items[i]
        queue.window:removeChildWindow(item)
        winMgr:destroyWindow(item)
    end
    queue.items = {}
    queue.first = 1
    queue.last = 1

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

-- Change the position of the queue.
-- The parameters are (in order) 'name of the queue', 'relative x-position', 'absolute x-position in pixel', 'relative y-position', 'absolute y-position in pixel'.
function P.moveQueue(queueName, relativeXPos, absoluteXPos, relativeYpos, absoluteYPos)
    local queueWindow = P.queueList[queueName].window
    queueWindow:setPosition(CEGUI.UVector2(CEGUI.UDim(relativeXPos, absoluteXPos), CEGUI.UDim(relativeYpos, absoluteYPos)))
end

-- Change the size of the queue.
-- The parameters are (in order) 'name of the queue', 'relative width', 'absolute width in pixel', 'relative height', 'absolute heigth in pixel'.
-- Additionally the last two parameters can be ommitted, which leads to the height being set such that all notifications can be displayed. using the size of the queue.
function P.resizeQueue(queueName, relativeWidth, absoluteWidth, relativeHeight, absoluteHeigth)
    local queue = P.queueList[queueName]
    local queueWindow = queue.window
    if queueWindow == nil then
        return
    end
    if absoluteHeigth == nil then
        absoluteHeigth = P.queueHeightHelper(queue, queue.maxSize)
        relativeHeight = 0
    end
    queueWindow:setSize(CEGUI.UVector2(CEGUI.UDim(relativeWidth, absoluteWidth), CEGUI.UDim(relativeHeight, absoluteHeigth)))
end

-- Change the horizontal alignment of the displayed notifications.
-- The parameters are the name of the queue and the alignment parameter,
function P.changeQueueAlignment(queueName, alignment)
    local queue = P.queueList[queueName]
    local queueWindow = queue.window
    if queueWindow == nil then
        return
    end

    queue.alignment = alignment
    local item = nil
    for i=queue.first,queue.last-1 do
        item = queue.items[i]
        item:setProperty("HorzFormatting", queue.alignment)
    end
end

-- Change the font size  of all notifications in a queue.
-- The parameters are (in order) 'name of the queue', 'font size'.
function P.changeQueueFontSize(queueName, size)
    local queue = P.queueList[queueName]
    local queueWindow = queue.window
    if queueWindow == nil then
        return
    end

    queue.fontSize = size
    for i=queue.first,queue.last-1 do
        P.setItemFontHelper(queue.items[i], queue, false)
    end
end

-- Change the font color of all notifications in a queue.
-- The parameters are (in order) 'name of the queue', 'ARGB of the font color in hex notation'.
function P.changeQueueFontColor(queueName, color)
    local queue = P.queueList[queueName]
    local queueWindow = queue.window
    if queueWindow == nil then
        return
    end

    queue.fontColor = color
    for i=queue.first,queue.last-1 do
        P.setItemFontHelper(queue.items[i], queue, true)
    end
end

-- Helper function to set the font size and color of a item of a queue.
-- The parameters are (in order) 'the ListboxItem', 'the queue table', 'whether color should be changed as well'
function P.setItemFontHelper(item, queue, changeColor)
    --local item = tolua.cast(item, "CEGUI::ListboxTextItem")
    local fontMgr = CEGUI.FontManager:getSingleton()
    if (fontMgr["isFontPresent"] and fontMgr:isFontPresent("BlueHighway-" .. queue.fontSize)) or -- cegui 0.6
        (fontMgr["isDefined"] and fontMgr:isDefined("BlueHighway-" .. queue.fontSize)) then -- cegui 0.7
        item:setFont("BlueHighway-" .. queue.fontSize)
    else
        orxonox.GUIManager:addFontHelper("BlueHighway-" .. queue.fontSize, queue.fontSize, "bluehigh.ttf")
        item:setFont("BlueHighway-" .. queue.fontSize)
    end
    if changeColor then
        item:setProperty("TextColours", "tl:" .. queue.fontColor .. " tr:" .. queue.fontColor .. " bl:" .. queue.fontColor .. " br:" .. queue.fontColor .. "")
    end
end

-- Helper function. Returns height a queue needs to have to display 'size' items.
function P.queueHeightHelper(queue, size)
    --local listbox = CEGUI.toListbox(queue.window)
    --local item = CEGUI.createListboxTextItem("Text")
    --P.setItemFontHelper(item, queue, false)
    --listbox:addItem(item)
    --local singleItemHeight = listbox:getTotalItemsHeight()
    local singleItemHeight = P.itemHeightHelper(queue)
    --local lookAndFeel = CEGUI.WidgetLookManager:getSingleton():getWidgetLook(queue.window:getLookNFeel())
    --local formattedArea = lookAndFeel:getNamedArea("ItemRenderingArea"):getArea():getPixelRect(queue.window)
    --local frameHeight = queue.window:getUnclippedOuterRect():getHeight() - formattedArea:getHeight()
    --listbox:removeItem(item)
    --return frameHeight + singleItemHeight*size
    return singleItemHeight*size + 1
end

function P.itemHeightHelper(queue)
    local item = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/NotificationLayer/Root/Test/")
    item:setText("text")
    P.setItemFontHelper(item, queue, true)
    queue.window:addChildWindow(item)
    item:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(1, 0)))
    item:setProperty("FrameEnabled", "false")
    local height = getStaticTextWindowHeight(item)
    queue.window:removeChildWindow(item)
    winMgr:destroyWindow(item)
    return height
end

return P
