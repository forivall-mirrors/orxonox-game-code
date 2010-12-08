-- DecisionPopup.lua

local P = createMenuSheet("DecisionPopup")

P.buttonList = {}

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = -2
    P.index = -1
end

function P.onLoad()

    --button are arranged in a 1x2 matrix
    local item = {
            ["button"] = winMgr:getWindow("orxonox/DecisionPopup_button_yes"),
            ["function"]  = P.button_yes
    }
    P.buttonList[1] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/DecisionPopup_button_no"),
            ["function"]  = P.button_no
    }
    P.buttonList[2] = item

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

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 1, 2)
end

return P

