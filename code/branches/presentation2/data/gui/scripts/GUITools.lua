function openDecisionPopup( text, callbackPtr )
    showGUI("DecisionPopup", false, true)
    DecisionPopup.setCallback(callbackPtr)
    DecisionPopup.setText(text)
end

function openInfoPopup( text, functionPtr )
    showGUI("InfoPopup", false, true)
    InfoPopup.setDo(functionPtr)
    InfoPopup.setText(text)
end
