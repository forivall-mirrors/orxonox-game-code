-- DecisionPopup.lua

local P = createMenuSheet("DecisionPopup")

function P.onLoad()

    --button are arranged in a 1x2 matrix
    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/DecisionPopup_button_yes"),
            ["callback"]  = P.button_yes
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/DecisionPopup_button_no"),
            ["callback"]  = P.button_no
    })
end

function P.onShow()
    P:setSelection(1, 1)
end

function P.setCallback(functionPtr)
    P.functionPtr = functionPtr
end

function P.setText( text )
    winMgr:getWindow("orxonox/DecisionPopup_text"):setText( text )
end

-- events for ingamemenu
function P.button_yes(e)
    if P.functionPtr ~= nil then
        P.functionPtr(true)
    end
    hideMenuSheet("DecisionPopup")
end

function P.button_no(e)
    if P.functionPtr ~= nil then
        P.functionPtr(false)
    end
    hideMenuSheet("DecisionPopup")
end

return P

