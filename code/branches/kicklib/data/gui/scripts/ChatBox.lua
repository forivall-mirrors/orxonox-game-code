-- ChatBox.lua

local P = createMenuSheet("ChatBox", true, TriBool.True, TriBool.Dontcare, false)

function P.ChatBoxCloseButton_clicked(e)
    orxonox.ChatInputHandler:getInstance():deactivate()
end

return P

