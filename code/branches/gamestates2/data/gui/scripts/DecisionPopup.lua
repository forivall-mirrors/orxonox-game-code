-- DecisionPopup.lua

local P = createSheet("DecisionPopup")

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
    hideGUI("DecisionPopup")
end

function P.button_no(e)
    if P.functionPtr ~= nil then
        P.functionPtr(false)
    end
    hideGUI("DecisionPopup")
end

return P

