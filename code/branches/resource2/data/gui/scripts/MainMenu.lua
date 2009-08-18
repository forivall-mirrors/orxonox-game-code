-- MainMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    MainMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "MainMenu"
P.layoutString = "MainMenu.layout"

function P:init()
    listbox = winMgr:getWindow("orxonox/LevelListbox")
    preselect = orxonox.LevelManager:getInstance():getDefaultLevel()
    orxonox.LevelManager:getInstance():compileAvailableLevelList()
    local levelList = {}
    local index = 0
    local level = ""
    while true do
      level = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
      if level == "" then
        break
      end
      table.insert(levelList, level)
      index = index + 1
    end
    table.sort(levelList)
    for k,v in pairs(levelList) do
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

return P

