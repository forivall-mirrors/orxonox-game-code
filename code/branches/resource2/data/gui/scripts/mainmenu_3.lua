-- mainmenu_2.lua
gui = require("gui")
local P = gui:new() --inherit everything from the gui package

mainmenu_2 = P

P.filename = "mainmenu_2"
P.layoutString = "MainMenu_2.layout"

function P:init()
    listbox = winMgr:getWindow("orxonox/LevelListbox")
    preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    orxonox.GUIManager:getInstance():getLevelList() --levellist variable ist set now
    table.sort(levellist)
    for k,v in pairs(levellist) do
        item = CEGUI.createListboxTextItem(v)        
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toListbox(listbox):addItem(item)
        if v .. ".oxw" == preselect then
            listbox:setItemSelectState(item, true)
        end
    end
end


-- events for mainmenu
function P.button_quit_clicked(e)
    hideGUI()
    orxonox.CommandExecutor:execute("exit")
end

function P.button_standalone_clicked(e)
    choice = winMgr:getWindow("orxonox/LevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.CommandExecutor:execute("startGame")
        toggleGUI()
    end
end

function P.button_server_clicked(e)
    choice = winMgr:getWindow("orxonox/LevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.CommandExecutor:execute("startServer")
        toggleGUI()
    end
end

function P.button_dedicated_clicked(e)
    choice = winMgr:getWindow("orxonox/LevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.CommandExecutor:execute("startDedicated")
        toggleGUI()
    end
end

function P.button_client_clicked(e)
    choice = winMgr:getWindow("orxonox/LevelListbox"):getFirstSelectedItem()
    if choice then
        orxonox.LevelManager:getInstance():setDefaultLevel(choice:getText() .. ".oxw")
        orxonox.CommandExecutor:execute("startClient")
        toggleGUI()
    end
end

function P.listbox_level_selectionchanged(e)
    if winMgr:getWindow("orxonox/LevelListbox"):getFirstSelectedItem() then
        winMgr:getWindow("orxonox/StandaloneButton"):enable()
    else
        winMgr:getWindow("orxonox/StandaloneButton"):disable()
    end
end

return mainmenu_2

