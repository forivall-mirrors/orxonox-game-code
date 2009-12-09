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
