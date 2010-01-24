-- CreditsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new("CreditsMenu")
if _REQUIREDNAME == nil then
    CreditsMenu = P
else
    _G[_REQUIREDNAME] = P
end

function P.CreditsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

