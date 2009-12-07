-- InfoPopup.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    InfoPopup = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "InfoPopup"
P.layoutString = "InfoPopup.layout"

function P:init()
end

function P.setDo(functionPtr)
    P.functionPtr = functionPtr
    if P.functionPtr ~= nil then
        P.functionPtr()
    end
end

function P.setText( text )
    winMgr:getWindow("orxonox/InfoPopup_text"):setText( text )
end

-- events for ingamemenu
function P.close(e)
    hideGUI("InfoPopup")
end

return P

