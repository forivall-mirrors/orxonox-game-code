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
    local height = window:getFont():getLineSpacing() + window:getUnclippedOuterRect():getHeight() - lookAndFeel:getNamedArea("WithFrameTextRenderArea"):getArea():getPixelRect(window):getHeight()
    local width =  window:getFont():getTextExtent(window:getText()) + window:getUnclippedOuterRect():getWidth() - lookAndFeel:getNamedArea("WithFrameTextRenderArea"):getArea():getPixelRect(window):getWidth()

    table.insert(size, height)
    table.insert(size, width)
    return size
end

function getScrollingStepSize(window)
    local height = window:getUnclippedOuterRect():getHeight()
    local maxHeight = CEGUI.System:getSingleton():getGUISheet():getUnclippedOuterRect():getHeight()
    local ratio = height/maxHeight
    return 0.008*ratio/0.3204
end

function getStaticTextWindowHeight(window)
    -- Get the area the text is formatted and drawn into.
    local lookAndFeel = CEGUI.WidgetLookManager:getSingleton():getWidgetLook(window:getLookNFeel())
    local formattedArea = lookAndFeel:getNamedArea("WithFrameTextRenderArea"):getArea():getPixelRect(window)
    -- Calculate the pixel height of the frame by subtracting the height of the area above from the total height of the window.
    local frameHeight = window:getUnclippedOuterRect():getHeight() - formattedArea:getHeight()

    local height = 0
    if ORXONOX_OLD_CEGUI then
        local lines = window:getFont():getFormattedLineCount(window:getText(), formattedArea, CEGUI.WordWrapLeftAligned)
        height = lines * window:getFont():getLineSpacing() + frameHeight
    else
        height = math.floor(CEGUI.PropertyHelper:stringToFloat(window:getProperty("VertExtent")) + frameHeight) + 1
    end
    return height
end
