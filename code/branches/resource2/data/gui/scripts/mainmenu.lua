layoutPath = "MainMenu.layout"

-- events for mainmenu
function button_quit_clicked(e)
  hideGUI()
  orxonox.CommandExecutor:execute("exit")
end

function button_standalone_clicked(e)
  orxonox.CommandExecutor:execute("startGame")
  toggleGUI()
end

function button_server_clicked(e)
  orxonox.CommandExecutor:execute("echo Not yet supported!")
  hideGUI()
end

function button_dedicated_clicked(e)
  orxonox.CommandExecutor:execute("echo Not yet supported!")
  hideGUI()
end

function button_client_clicked(e)
  orxonox.CommandExecutor:execute("echo Not yet supported!")
  hideGUI()
end
