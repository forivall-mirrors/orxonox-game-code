-- Returns a new menu sheet
-- See MenuSheet.new for details about the parameters
function createMenuSheet(name, bHidePrevious, tShowCursor, tUseKeyboard, bBlockJoyStick)
    local sheet = require("MenuSheet").new(name, bHidePrevious, tShowCursor, tUseKeyboard, bBlockJoyStick)
    global(sheet.name)
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