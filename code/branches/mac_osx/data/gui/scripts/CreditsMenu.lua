-- CreditsMenu.lua

local P = createMenuSheet("CreditsMenu")

function P.CreditsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

