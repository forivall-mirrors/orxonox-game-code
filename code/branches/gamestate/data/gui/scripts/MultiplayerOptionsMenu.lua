-- MultiplayerOptionsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    MultiplayerOptionsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "MultiplayerOptionsMenu"
P.layoutString = "MultiplayerOptionsMenu.layout"

function P.MultiplayerOptionsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P
