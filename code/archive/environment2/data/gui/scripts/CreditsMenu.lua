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
    description:setProperty("HorzFormatting", "WordWrapLeftAligned")
    description:setProperty("VertFormatting", "TopAligned")
    description:setText("                                   Orxonox version 0.0.3 \n\n       Programming Language:  C++ - lua - tcl \n       Environment:  OpenAL - Ogre - OIS - CEGUI - enet - ODE \n       Licence:  GNU General Public Licence - Creative Commons \n       http://www.orxonox.net \n \n       Credits:")

    description:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
    height = getStaticTextWindowHeight(description)
    description:setHeight(CEGUI.UDim(0, height))
end

function P.CreditsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

