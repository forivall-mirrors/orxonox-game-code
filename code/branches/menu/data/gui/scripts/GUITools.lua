function openDecisionPopup( text, callbackPtr )
    orxonox.CommandExecutor:execute("showGUI DecisionPopup false true")
--     showGUI("DecisionPopup", false, true)
    DecisionPopup.setCallback(InGameMenu.callback)
    DecisionPopup.setText(text)
end