gui = require("BasicGUI")
local P = BasicGUI:new("QuestGUI")
if _REQUIREDNAME == nil then
    QuestGUI = P
else
    _G[_REQUIREDNAME] = P
end

function P:show()
    self.window:show() -- TDO: Do this through parent...
    self.visible = true

    local questManager = orxonox.QuestManager:getInstance()

    local questsList = winMgr:getWindow("orxonox/QuestGUI/QuestsList")

    local window = questManager:getQuestGUI(P.filename)

    questsList:addChildWindow(window)

end

return P

