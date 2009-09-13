-- QuestGUI

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    QuestGUI = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "QuestGUI"
P.layoutString = "QuestGUI.layout"

local windowManager = CEGUI.WindowManager:getSingleton()
-- local playerManager = orxonox.PlayerManager
local questManager = orxonox.QuestManager:getInstance()

local quests = {}

-- overwrites load function of previous load function
function P:load()
    element = P.__index.load(P) -- calling load function of parent class with ourselves and do other stuff now
    --win = winMgr:getWindow("orxonox/LevelListBox")
    --print(win.getItemCount())
    --item = winMgr:createWindow("TaharezLook/ListboxItem", "orxonox/LevelListBoxItem1")
    --item:setText("HiHo")
    --win:addItem(item)
    --print(win:getItemCount())
    return element
end

function P:show()
    self.window:show() -- TDO: Do this through parent...
    self.visible = true

    local questsList = windowManager:getWindow("orxonox/QuestGUI/QuestsList")

    local window = questManager:getQuestGUI(P.filename)

    questsList:addChildWindow(window)

end

function P:button_activeQuests_clicked(e)
    
end

function P:button_completedQuests_clicked(e)
    
end

function P:button_failedQuests_clicked(e)
    
end

return P

