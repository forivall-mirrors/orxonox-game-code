-- CreditsMenu.lua

local P = createMenuSheet("CreditsMenu")

P.scrollbarWidth = 13

function P.onLoad()
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/CreditsBackButton"),
            ["callback"]  = P.CreditsBackButton_clicked
    })
end

function P.onShow()
    local description = winMgr:getWindow("orxonox/CreditsText")

    height = getStaticTextWindowHeight(description)
    description:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(0.0, height)))
end

function P.CreditsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

