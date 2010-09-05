-- NotificationLayer.lua

local P = createMenuSheet("NotificationLayer")

P.queueList = {}
P.nameList = {}
P.visible = nil
P.editMode = false
P.editList = {}

function P.createQueue(name, size)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    local queue = winMgr:createWindow("MenuWidgets/Listbox", "orxonox/NotificationLayer/Root/Queue/" .. name)
    queue:setProperty("BackgroundColor", "00FFFFFF")
    root:addChildWindow(queue)

    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    queue:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, 0), CEGUI.UDim(0, P.queueHeightHelper(queue, size))))

    P.setVisible(queue, false)

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

    if P.visible == false then
        P.setVisible(queue, true)
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

    if listbox:getItemCount() == 0 then
        P.setVisible(queue, false)
    end
end

function P.removeNotification(queueName, index)
    local queue = P.nameToQueueHelper(queueName)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    listbox:removeItem(listbox:getListboxItemFromIndex(tonumber(index)))

    if listbox:getItemCount() == 0 then
        P.setVisible(queue, false)
    end
end

function P.clearQueue(name)
    local queue = P.nameToQueueHelper(name)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    CEGUI.toListbox(queue):resetList()

    P.setVisible(queue, false)
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

function P.setVisible(queue, visible)
    queue:setVisible(visible)
    P.visible = visible
end

function P.enterEditMode()
    P.editMode = true

    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    --Replace all queues with FrameWindows
    for k,v in pairs(P.queueList) do
        if v ~= nil then
            root:removeChildWindow(v)
            local frame = winMgr:createWindow("MenuWidgets/FrameWindow", "orxonox/NotificationLayer/Root/EditMode/" .. P.nameList[k])
            frame:setArea(v:getArea())
            root:addChildWindow(frame)
            P.editList[k] = frame
        end
    end
end

function P.leaveEditMode()
    P.editMode = false

    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    --Replace all queues with FrameWindows
    for k,v in pairs(P.queueList) do
        if v ~= nil then
            root:addChildWindow(v)
            v:setArea(P.editList[k]:getArea())
            winMgr:destroyWindow(P.editList[k])
            P.editList[k] = nil
        end
    end

    showMenuSheet(P.name, false, true)
end

function P.onHide()
    if P.editMode then
        P.leaveEditMode()
    end
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

