-- ChatBox.lua

local P = createMenuSheet("ChatBox")

function P.onLoad()
  local window = winMgr:getWindow("orxonox/ChatBox")
  orxonox.GUIManager:subscribeEventHelper(window, "CloseClicked", P.name .. ".ChatBoxCloseButton_clicked")
end

function P.ChatBoxCloseButton_clicked(e)
    orxonox.ChatInputHandler:getInstance():deactivate()
end

return P

