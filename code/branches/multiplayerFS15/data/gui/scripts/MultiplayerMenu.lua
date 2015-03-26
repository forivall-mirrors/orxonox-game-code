-- MultiplayerMenu.lua

local P = createMenuSheet("MultiplayerMenu")

--joinMode is 1 for choice "LAN" and 2 for "Internet"
--initial status 1
P.joinMode = 1

function P.onLoad()
    P.multiplayerMode = "startClient"

    --button are arranged in a 3x2 matrix, Join and Host buttons are in the upper left and middle, the back button in the lower right of the table
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerJoinButton"),
            ["callback"]  = P.MultiplayerJoinButton_clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerHostButton"),
            ["callback"]  = P.MultiplayerHostButton_clicked
    })

    P:setButton(2, 3, {
            ["button"] = winMgr:getWindow("orxonox/MultiplayerBackButton"),
            ["callback"]  = P.MultiplayerBackButton_clicked
    })
end

function P.onShow()
    --P.showServerList()

    if P.joinMode == 1 then
        local window = winMgr:getWindow("orxonox/MultiplayerLanButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
    if P.joinMode == 2 then
        local window = winMgr:getWindow("orxonox/MultiplayerInternetButton")
        local button = tolua.cast(window,"CEGUI::RadioButton")
        button:setSelected(true)
    end
end

function P.LanButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local button = tolua.cast(we.window,"CEGUI::RadioButton")
    P.joinMode = 1
    if button:isSelected() == true then
        P.showServerList()
    end
end

function P.InternetButton_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local button = tolua.cast(we.window,"CEGUI::RadioButton")
    P.joinMode = 2
    if button:isSelected() == true then
        P.showServerList()
    end
end

function P.MultiplayerHostButton_clicked(e)
    showMenuSheet("HostMenu", true)
end


function P.MultiplayerJoinButton_clicked(e)
    local choice = winMgr:getWindow("orxonox/MultiplayerListbox"):getFirstSelectedItem()
    local destination = nil
    if choice then
        local index = tolua.cast(choice, "CEGUI::ListboxItem"):getID()
        destination = P.serverList[index][2]
    else
        return
    end
    orxonox.execute("startClient " .. destination)
    hideAllMenuSheets()
end

function P.MultiplayerBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.showServerList()
    -- LAN Discovery
    if P.joinMode == 1 then
        local listbox = winMgr:getWindow("orxonox/MultiplayerListbox")
        CEGUI.toListbox(listbox):resetList()
        local discovery = orxonox.LANDiscovery:getInstance()
        discovery:discover()
        P.serverList = {}
        local index = 0
        local servername = ""
        local serverip = ""
        local serverrtt = ""
        while true do
            servername = discovery:getServerListItemName(index)
            if servername == "" then
                break
            end
            serverip = discovery:getServerListItemIP(index)
            if serverip == "" then
                break
            end
            serverrtt = discovery:getServerListItemRTT(index)

            table.insert(P.serverList, {servername, serverip, serverrtt})
            index = index + 1
        end
        index = 1
        for k,v in pairs(P.serverList) do
            local item = CEGUI.createListboxTextItem( v[1] .. ": " .. v[2] .. " Ping: " .. v[3] .."ms" )
            item:setID(index)
            index = index + 1
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            CEGUI.toListbox(listbox):addItem(item)
        end
    -- WAN Discovery
    elseif P.joinMode == 2 then
        local listbox = winMgr:getWindow("orxonox/MultiplayerListbox")
        CEGUI.toListbox(listbox):resetList()
        local discovery = orxonox.WANDiscovery()
        orxout("discovering." )
        discovery:discover()
        orxout("discovered." )
        P.serverList = {}
        local index = 0
        local servername = ""
        local serverip = ""
        local serverrtt = ""
        while true do
            servername = discovery:getServerListItemName(index)
            if servername == "" then
                break
            end
            serverip = discovery:getServerListItemIP(index)
            if serverip == "" then
                break
            end
            serverrtt = discovery:getServerListItemRTT(index)

            table.insert(P.serverList, {servername, serverip, serverrtt})
            index = index + 1
        end
        index = 1
        for k,v in pairs(P.serverList) do
            local item = CEGUI.createListboxTextItem( v[1] .. ": " .. v[2] .. " Ping: " .. v[3] .."ms" )
            item:setID(index)
            index = index + 1
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            CEGUI.toListbox(listbox):addItem(item)
        end
    end

end

return P
