function openDecisionPopup( text, callbackPtr )
    showGUI("DecisionPopup", false, true)
    DecisionPopup.setCallback(callbackPtr)
    DecisionPopup.setText(text)
end

function openInfoPopup(text, functionPtr, closeButton, arguments)
    showGUI("InfoPopup", false, true)
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
