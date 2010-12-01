-- MultiplayerMenu.lua

local P = createMenuSheet("MultiplayerMenu")

P.buttonList = {}

function P.onLoad()
    P.multiplayerMode = "startClient" 

    --button are arranged in a 2x2 matrix, the left lower item is nil
    local item = {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerJoinButton"),
            ["function"]  = P.MultiplayerJoinButton_clicked
    }
    P.buttonList[1] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerHostButton"),
            ["function"]  = P.MultiplayerHostButton_clicked
    }
    P.buttonList[2] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerBackButton"),
            ["function"]  = P.MultiplayerBackButton_clicked
    }
    P.buttonList[4] = item

end

function P.onShow()
    P.showServerList()

    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
end

function P.MultiplayerHostButton_clicked(e)
    showMenuSheet("HostMenu", true)
end


function P.MultiplayerJoinButton_clicked(e)
    local choice = winMgr:getWindow("orxonox/MultiplayerListbox"):getFirstSelectedItem()
    if choice then
        local client = orxonox.Client:getInstance()
        local index = tolua.cast(choice, "CEGUI::ListboxItem"):getID()
        client:setDestination( P.serverList[index][2], 55556 )
    else
        return
    end
    orxonox.execute("startClient")
    hideAllMenuSheets()
end

function P.MultiplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.showServerList()
    local listbox = winMgr:getWindow("orxonox/MultiplayerListbox")
    CEGUI.toListbox(listbox):resetList()
    local discovery = orxonox.LANDiscovery:getInstance()
    discovery:discover()
    P.serverList = {}
    local index = 0
    local servername = ""
    local serverip = ""
    while true do
        servername = discovery:getServerListItemName(index)
        if servername == "" then
            break
        end
        serverip = discovery:getServerListItemIP(index)
        if serverip == "" then
          break
        end
        table.insert(P.serverList, {servername, serverip})
        index = index + 1
    end
    index = 1
    for k,v in pairs(P.serverList) do
        local item = CEGUI.createListboxTextItem( v[1] .. ": " .. v[2] )
        item:setID(index)
        index = index + 1
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
    end
end

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 2, 2)
end

return P

