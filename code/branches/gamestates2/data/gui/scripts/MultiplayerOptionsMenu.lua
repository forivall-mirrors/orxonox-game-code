-- MultiplayerOptionsMenu.lua

local P = createMenuSheet("MultiplayerOptionsMenu")

function P.MultiplayerOptionsBackButton_clicked(e)
    hideGUI(P.name)
end

return P
