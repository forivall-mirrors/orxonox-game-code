-- CreditsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    CreditsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "CreditsMenu"
P.layoutString = "CreditsMenu.layout"

function P.CreditsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

