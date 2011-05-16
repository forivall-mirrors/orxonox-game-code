-- DockingDialog.lua

local P = createMenuSheet("DockingDialog")

P.docks = {}

function P.onLoad()
    
end

function P.onShow()
    orxonox.CommandExecutor:execute("pause")
    P.update()
end

function P.onHide()
    orxonox.CommandExecutor:execute("pause")
end

function P.update()
    -- update dock list
    P.docks = {}
    local docks = orxonox.Dock:getNumberOfActiveDocks()
    for i = 0, docks-1 do
        table.insert(P.docks, orxonox.Dock:getActiveDockAtIndex(i))
    end

    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/DockingDocks"))
    listbox:resetList()

    for k in pairs(P.docks) do
        local item = CEGUI.createListboxTextItem("Dock " .. k)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        listbox:addItem(item)
    end
end

function P.dockButton_clicked(e)
    local listbox = CEGUI.toListbox(winMgr:getWindow("orxonox/DockingDocks"))
    local choice = listbox:getFirstSelectedItem()
    if choice ~= nil then
        local index = listbox:getItemIndex(choice)
        local dock = P.docks[index+1]
        cout(0, index )--.. ": " .. P.docks[index])
        if dock ~= nil then
            cout(0, "LUA>Docking")
            dock:dock()
        end
    end
    hideMenuSheet(P.name)
end

function P.cancelButton_clicked(e)
    --P.hideMe()
    hideMenuSheet(P.name)
end

return P
