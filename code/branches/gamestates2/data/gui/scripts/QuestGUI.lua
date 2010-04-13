-- QuestGUI.lua

local P = createMenuSheet("QuestGUI")

function P.show()
    P.window:show() -- TDO: Do this through parent...
    P.visible = true

    local questManager = orxonox.QuestManager:getInstance()

    local questsList = winMgr:getWindow("orxonox/QuestGUI/QuestsList")

    local window = questManager:getQuestGUI(P.name)

    questsList:addChildWindow(window)

end

return P

