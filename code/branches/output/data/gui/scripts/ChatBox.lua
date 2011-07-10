-- ChatBox.lua

local P = createMenuSheet("ChatBox", true, tribool(true), tribool(dontcare), false)

function P.ChatBoxCloseButton_clicked(e)
    orxonox.ChatInputHandler:getInstance():deactivate()
end

return P

