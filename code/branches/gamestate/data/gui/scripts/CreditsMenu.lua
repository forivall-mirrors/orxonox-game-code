-- CreditsMenu.lua

local sheetName = _REQUIREDNAME or "CreditsMenu"
-- Create object of type BasicGUI and make it global
local P = require("BasicGUI"):new(sheetName)
_G[sheetName] = P

function P.CreditsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

