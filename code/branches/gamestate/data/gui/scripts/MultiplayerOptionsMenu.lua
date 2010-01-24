-- MultiplayerOptionsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new("MultiplayerOptionsMenu")
if _REQUIREDNAME == nil then
    MultiplayerOptionsMenu = P
else
    _G[_REQUIREDNAME] = P
end

function P.MultiplayerOptionsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P
