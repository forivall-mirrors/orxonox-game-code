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

function P.execute(functionPtr, arguments)
    if functionPtr ~= nil then
        if arguments ~= nil then
            functionPtr(arguments)
        else
            functionPtr()
        end
    end
end

function P.setText( text )
    winMgr:getWindow("orxonox/InfoPopup_text"):setText( text )
end

function P.setCloseButton(closeButton)
    close = winMgr:getWindow("orxonox/InfoPopup_close")
    close:setVisible(closeButton)
    if(not closeButton) then
        close:deactivate();
    else
        close:activate();
    end
end

-- events for ingamemenu
function P.close(e)
    hideGUI("InfoPopup")
end

return P
