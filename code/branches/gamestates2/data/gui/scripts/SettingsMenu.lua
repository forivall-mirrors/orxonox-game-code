-- SettingsMenu.lua

local P = createSheet("SettingsMenu")

function P.SettingsGameplayButton_clicked(e)
    showGUI("GameplayMenu", true)
end

function P.SettingsMultiplayerOptionsButton_clicked(e)
    showGUI("MultiplayerOptionsMenu", true)
end

function P.SettingsControlsButton_clicked(e)
    showGUI("ControlsMenu", true)
end

function P.SettingsGraphicsButton_clicked(e)
    showGUI("GraphicsMenu", true)
end

function P.SettingsAudioButton_clicked(e)
    showGUI("AudioMenu", true)
end

function P.SettingsResetSettingsButton_clicked(e)
    -- reset settings
    logMessage(0, "event: reset settings")
end

function P.SettingsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

