-- InfoPopup.lua

local P = createMenuSheet("InfoPopup")

function P.execute(functionPtr, arguments)
    if functionPtr ~= nil then
        if arguments ~= nil then
            functionPtr(arguments)
        else
            functionPtr()
        end
    end
end

function P.setText(text)
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
    hideMenuSheet(P.name)
end

return P
