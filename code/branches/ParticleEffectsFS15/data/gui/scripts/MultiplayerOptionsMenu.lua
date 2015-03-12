-- MultiplayerOptionsMenu.lua

local P = createMenuSheet("MultiplayerOptionsMenu")

function P.MultiplayerOptionsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P
