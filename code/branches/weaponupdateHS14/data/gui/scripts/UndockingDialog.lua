-- DockingDialog.lua

local P = createMenuSheet("UndockingDialog")

P.docks = {}

function P.onLoad()

     --button are arranged in a 1x2 matrix
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/Undocking/DockButton"),
            ["callback"]  = P.undockButton_clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/Undocking/CancelButton"),
            ["callback"]  = P.cancelButton_clicked
    })
    
end

function P.onShow()
    orxonox.execute("setPause 1")
    P.update()
end

function P.onHide()
    orxonox.execute("setPause 0")
end

function P.update()
    -- update dock list
    P.docks = {}
    local docks = orxonox.Dock:getNumberOfActiveDocks()
    for i = 0, docks-1 do
        table.insert(P.docks, orxonox.Dock:getActiveDockAtIndex(i))
    end

    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/Undocking/Docks"))
    listbox:resetList()

    for k in pairs(P.docks) do
        local item = CEGUI.createListboxTextItem("Dock " .. k)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        listbox:addItem(item)
        if k == 1 then
            listbox:setItemSelectState(item, true)
        end
    end
end

function P.undockButton_clicked(e)
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/Undocking/Docks"))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        local index = listbox:getItemIndex(choice)
        local dock = P.docks[index+1]
        if dock ~= nil then
            dock:undock()
        end
    end
    hideMenuSheet(P.name)
end

function P.cancelButton_clicked(e)
    hideMenuSheet(P.name)
end

return P
