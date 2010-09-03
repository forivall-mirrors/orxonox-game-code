-- NotificationLayer.lua

local P = createMenuSheet("NotificationLayer")

P.queueList = {}
P.nameList = {}

function P.createQueue(name, size)
    local root = winMgr:getWindow("orxonox/NotificationLayer/Root")
    local queue = winMgr:createWindow("MenuWidgets/Listbox", "orxonox/NotificationLayer/Root/Queue/" .. name)
    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 0)))
    queue:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0.2, 0)))
    root:addChildWindow(queue)
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

function P.changePosition(name, xPos, yPos)
    local queue = P.nameToQueue(name)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    queue:setPosition(CEGUI.UVector2(CEGUI.UDim(tonumber(xPos), 0), CEGUI.UDim(tonumber(yPos), 0)))
end

function P.pushNotification(queueName, notification)
    local queue = P.nameToQueue(queueName)
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
    local queue = P.nameToQueue(queueName)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    listbox:removeItem(listbox:getListboxItemFromIndex(listbox:getItemCount()-1))
end

function P.removeNotification(queueName, index)
    local queue = P.nameToQueue(queueName)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    listbox:removeItem(listbox:getListboxItemFromIndex(tonumber(index)))
end

function P.clearQueue(name)
    local queue = P.nameToQueue(name)
    if queue == nil then
        cout(0, "Queue is nil!")
        return
    end
    local listbox = CEGUI.toListbox(queue)
    CEGUI.toListbox(queue):resetList()
end

function P.nameToQueue(name)
    local queue = nil
    for k,v in pairs(P.nameList) do
        if v == name then
            queue = P.queueList[k]
            break
        end
    end
    return queue
end

return P

