-- NotificationLayer.lua

local P = createMenuSheet("NotificationLayer")

P.queueList = {}
P.nameList = {}

function P.createQueue(name, size)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    local queue = winMgr:createWindow("MenuWidgets/Listbox", "orxonox/NotificationLayer/Root/Queue/" .. name)
    queue:setProperty("Alpha", 0.3)
    queue:setProperty("InheritsAlpha", "setFalse")
    root:addChildWindow(queue)

    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    queue:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, 0), CEGUI.UDim(0, P.queueHeightHelper(queue, size))))

    table.insert(P.queueList, queue)
    table.insert(P.nameList, name)
    --TODO: Check name for uniqueness.
end

function P.removeQueue(name)
    local queue = nil

    for k,v in pairs(P.nameList) do
        if v == name then
            P.nameList[k] = nil
            queue = P.queueList[k]
            P.queueList[k] = nil
            break
        end
    end

    if queue ~= nil then
        winMgr:destroyWindow(queue)
    end
end

function P.pushNotification(queueName, notification)
    local queue = P.nameToQueueHelper(queueName)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    item = CEGUI.createListboxTextItem(notification)
    local listbox = CEGUI.toListbox(queue)
    if listbox:getItemCount() == 0 then
        listbox:addItem(item)
    else
        listbox:insertItem(item, listbox:getListboxItemFromIndex(0))
    end
end

function P.popNotification(queueName)
    local queue = P.nameToQueueHelper(queueName)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    listbox:removeItem(listbox:getListboxItemFromIndex(listbox:getItemCount()-1))
end

function P.removeNotification(queueName, index)
    local queue = P.nameToQueueHelper(queueName)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    listbox:removeItem(listbox:getListboxItemFromIndex(tonumber(index)))
end

function P.clearQueue(name)
    local queue = P.nameToQueueHelper(name)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    CEGUI.toListbox(queue):resetList()
end

function P.changePosition(name, xPos, yPos)
    local queue = P.nameToQueueHelper(name)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(tonumber(xPos), 0), CEGUI.UDim(tonumber(yPos), 0)))
    queue:setWidth(CEGUI.UDim(1.0, -xPos))
end

function P.changeSize(name, size)
    local queue = P.nameToQueueHelper(name)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    queue:setHeight(CEGUI.UDim(0, P.queueHeightHelper(queue, size)))
end

function P.nameToQueueHelper(name)
    local queue = nil
    for k,v in pairs(P.nameList) do
        if v == name then
            queue = P.queueList[k]
            break
        end
    end
    return queue
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

