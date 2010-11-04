-- ChatBox.lua

local P = createMenuSheet("ChatBox")

function P.ChatBoxCloseButton_clicked(e)
    orxonox.ChatInputHandler:getInstance():deactivate()
end

return P

