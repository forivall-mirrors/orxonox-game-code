-- Returns a new menu sheet
-- See MenuSheet.new for details about the parameters
function createMenuSheet(name, bHidePrevious, tShowCursor, tUseKeyboard, bBlockJoyStick)
    local sheet = require("MenuSheet").new(name, bHidePrevious, tShowCursor, tUseKeyboard, bBlockJoyStick)
    _G[sheet.name] = sheet -- Global access required because of the event handlers
    return sheet
end

-- Returns a new HUD sheet
function createHUDSheet(name)
    local sheet = require("HUDSheet").new(name)
    _G[sheet.name] = sheet -- Global access required because of the event handlers
    return sheet
end

function openDecisionPopup( text, callbackPtr )
    showMenuSheet("DecisionPopup", false, true)
    DecisionPopup.setCallback(callbackPtr)
    DecisionPopup.setText(text)
end

function openInfoPopup(text, functionPtr, closeButton, arguments)
    showMenuSheet("InfoPopup", false, true)
    InfoPopup.execute(functionPtr, arguments)
    InfoPopup.setText(text)
    InfoPopup.setCloseButton(closeButton)
end

function getMinTextSize(window)
    local size = {}

    local lookAndFeel = CEGUI.WidgetLookManager:getSingleton():getWidgetLook(window:getLookNFeel())
    local height = window:getFont():getLineSpacing() + window:getUnclippedPixelRect():getHeight() - lookAndFeel:getNamedArea("WithFrameTextRenderArea"):getArea():getPixelRect(window):getHeight()
    local width =  window:getFont():getTextExtent(window:getText()) + window:getUnclippedPixelRect():getWidth() - lookAndFeel:getNamedArea("WithFrameTextRenderArea"):getArea():getPixelRect(window):getWidth()

    table.insert(size, height)
    table.insert(size, width)
    return size
end

function getScrollingStepSize(window)
    local height = window:getUnclippedPixelRect():getHeight()
    local maxHeight = CEGUI.System:getSingleton():getGUISheet():getUnclippedPixelRect():getHeight()
    local ratio = height/maxHeight
    return 0.008*ratio/0.3204
end

function getStaticTextWindowHeight(window)
    local lookAndFeel = CEGUI.WidgetLookManager:getSingleton():getWidgetLook(window:getLookNFeel())
    local formattedArea = lookAndFeel:getNamedArea("WithFrameTextRenderArea"):getArea():getPixelRect(window)
    local frameHeight = window:getUnclippedPixelRect():getHeight() - formattedArea:getHeight()
    local lines = window:getFont():getFormattedLineCount(window:getText(), formattedArea, CEGUI.WordWrapLeftAligned)
    local height = lines * window:getFont():getLineSpacing() + frameHeight
    return height
end

--function to iterate through a menu sheet by using arrowkeys

--@arguments:
--  list: 2-dimensional table, arguments are items that contain a button and its function
--        !!note: each button can only be in the list once!!
--  code: code of any key on the keyboard
--  P: menusheet
--  n: number of rows of the buttontable
--  m: number of colums of the buttontable

function buttonIteratorHelper(list, code, P, n, m)

    --after a key (down,up,left,right) is pressed the index of the current button has to be updated    

    --key down
    if code == "208" then
        if P.index < 0 then     -- initial status
            P.index = 0
            P.oldindex = -1
        else
            P.oldindex = P.index
            P.index = (P.index + m) % (m*n)     --modulo operation works as a "wrap around" in the button menu
                                                
            while list[P.index+1] == nil do     
                P.oldindex = P.index
                P.index = (P.index + m) % (m*n)
            end
        end

    --key up
    elseif code == "200" then
        if P.index < 0 then
            P.index = 0
            P.oldindex = -1
        elseif(P.index == 0) then
            P.oldindex = P.index
            P.index = m*n-m

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = (P.index-m)%(m*n)
            end
        else
            P.oldindex = P.index
            P.index = (P.index -m) % (m*n)

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = (P.index-m)%(m*n)
            end
        end

    --key right
    elseif code == "205" then
        if P.index < 0 then
            P.index = 0
            P.oldindex = -1
        elseif (P.index+1) % m == 0 then     -- we are at the right-end of a row
            P.oldindex = P.index
            P.index = P.index + 1 -m

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = P.index + 1
            end
        else
            P.oldindex = P.index
            P.index = P.index + 1

            while list[P.index+1] == nil do
                if (P.index+1) % m == 0 then     -- we are at the right-end of a row
                    P.oldindex = P.index
                    P.index = P.index + 1-m

                else    
                    P.oldindex = P.index
                    P.index = P.index + 1
                end
            end
        end    

    --key left
    elseif code == "203" then
        if P.index < 0 then
            P.index = 0
            P.oldindex = -1
        elseif P.index % m == 0 then         -- we are at the left-end of a row 
            P.oldindex = P.index
            P.index = P.index +m-1

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = P.index -1
            end
        else
            P.oldindex = P.index
            P.index = P.index -1

            while list[P.index+1] == nil do
                if P.index % m == 0 then     -- we are at the left-end of a row 
                    P.oldindex = P.index
                    P.index = P.index -1 + m
                else                
                    P.oldindex = P.index
                    P.index = P.index -1
                end
            end   
        end
    end
       
    --to update the new current button
    if (code == "208" or code == "200" or code == "203" or code == "205") and P.oldindex~= P.index then

        local system = CEGUI.System:getSingleton()
        local window = winMgr:getWindow("orxonox/MainMenuBackground")

        local item = list[P.index+1]
        local child = item["button"] 

        --teste ob der Button nicht schon gehighlightet ist
        if child:getProperty("NormalImageRightEdge") == "set:TaharezGreenLook image:ButtonRightHighlight" then
            --nop
        else
            child:setProperty("NormalImageRightEdge", string.sub(child:getProperty("NormalImageRightEdge"),1,-7) .. "Highlight")
            child:setProperty("NormalImageLeftEdge", string.sub(child:getProperty("NormalImageLeftEdge"),1,-7) .. "Highlight")
            child:setProperty("NormalImageBackground", string.sub(child:getProperty("NormalImageBackground"),1,-7) .. "Highlight")
            if P.oldindex >= 0 then
                if list[P.oldindex+1] ~= nil then
                    local item = list[P.oldindex+1]
                    local oldChild = item["button"]
                    oldChild:setProperty("NormalImageRightEdge", string.sub(oldChild:getProperty("NormalImageRightEdge"),1,-10) .. "Normal")
                    oldChild:setProperty("NormalImageLeftEdge", string.sub(oldChild:getProperty("NormalImageLeftEdge"),1,-10) .. "Normal")
                    oldChild:setProperty("NormalImageBackground", string.sub(oldChild:getProperty("NormalImageBackground"),1,-10) .. "Normal")
                end
            end
        end

        --for every highlighted button check if index is on its position. If not, set imageproperty on "normal"
        local i = 1
        while i < (n*m) do
            if i == P.index +1 then 
                i = i+1
            else
                if list[i] ~= nil then 
                local item = list[i]
                local child = item["button"]
                    if child:getProperty("NormalImageRightEdge") == "set:TaharezGreenLook image:ButtonRightHighlight" then
                        child:setProperty("NormalImageRightEdge", string.sub(child:getProperty("NormalImageRightEdge"),1,-10) .. "Normal")
                        child:setProperty("NormalImageLeftEdge", string.sub(child:getProperty("NormalImageLeftEdge"),1,-10) .. "Normal")
                        child:setProperty("NormalImageBackground", string.sub(child:getProperty("NormalImageBackground"),1,-10) .. "Normal")
                    end
                end
            end
                i=i+1
        end
    end
    
    --enter
    if code == "28" then
        local item = list[P.index+1]
        local child = item["button"] 
        child:setProperty("NormalImageRightEdge", string.sub(child:getProperty("NormalImageRightEdge"),1,-10) .. "Normal")
        child:setProperty("NormalImageLeftEdge", string.sub(child:getProperty("NormalImageLeftEdge"),1,-10) .. "Normal")
        child:setProperty("NormalImageBackground", string.sub(child:getProperty("NormalImageBackground"),1,-10) .. "Normal")

        local foo = item["function"]
        foo()
    end

end

--write index and oldindex on the console
--works like buttonIteratorHelper
function indexTester(list,code,P,n,m)
    --key down
    if code == "208" then
        if P.index < 0 then     -- initial status
            P.index = 0
            P.oldindex = -1
        else
            P.oldindex = P.index
            P.index = (P.index + m) % (m*n)

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = (P.index + m) % (m*n)
            end
        end

    --key up
    elseif code == "200" then
        if P.index < 0 then
            P.index = 0
            P.oldindex = -1
        elseif(P.index == 0) then
            P.oldindex = P.index
            P.index = m*n-m

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = (P.index-m)%(m*n)
            end
        else
            P.oldindex = P.index
            P.index = (P.index -m) % (m*n)

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = P.index -m
            end
        end

    --key right
    elseif code == "205" then
        if P.index < 0 then
            P.index = 0
            P.oldindex = -1
        elseif (P.index+1) % m == 0 then     -- we are at the right-end of a row
            P.oldindex = P.index
            P.index = P.index + 1 -m

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = P.index + 1
            end
        else
            P.oldindex = P.index
            P.index = P.index + 1

            while list[P.index+1] == nil do
                if (P.index+1) % m == 0 then     -- we are at the right-end of a row
                    P.oldindex = P.index
                    P.index = P.index + 1-m

                else    
                    P.oldindex = P.index
                    P.index = P.index + 1
                end
            end
        end    

    --key left
    elseif code == "203" then
        if P.index < 0 then
            P.index = 0
            P.oldindex = -1
        elseif P.index % m == 0 then         -- we are at the left-end of a row 
            P.oldindex = P.index
            P.index = P.index +m-1

            while list[P.index+1] == nil do
                P.oldindex = P.index
                P.index = P.index -1
            end
        else
            P.oldindex = P.index
            P.index = P.index -1

            while list[P.index+1] == nil do
                if P.index % m == 0 then     -- we are at the left-end of a row 
                    P.oldindex = P.index
                    P.index = P.index -1 + m
                else                
                    P.oldindex = P.index
                    P.index = P.index -1
                end
            end   
        end
    end    

    cout(0, P.oldindex)
    cout(0, P.index)

end




