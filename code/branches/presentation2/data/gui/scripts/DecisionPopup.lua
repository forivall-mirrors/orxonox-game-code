-- DecisionPopup.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    DecisionPopup = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "DecisionPopup"
P.layoutString = "DecisionPopup.layout"

function P:init()
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
    hideGUI("DecisionPopup")
end

function P.button_no(e)
    if P.functionPtr ~= nil then
        P.functionPtr(false)
    end
    hideGUI("DecisionPopup")
end

return P

