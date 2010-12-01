-- CreditsMenu.lua

local P = createMenuSheet("CreditsMenu")

P.buttonList = {}

function P.onLoad()
    local item = {
            ["button"] = winMgr:getWindow("orxonox/CreditsBackButton"),
            ["function"]  = P.CreditsBackButton_clicked
    }
    P.buttonList[1] = item
end

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
end

function P.CreditsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 1, 1)
end

return P

