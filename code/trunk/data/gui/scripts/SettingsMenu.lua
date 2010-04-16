-- SettingsMenu.lua

local P = createMenuSheet("SettingsMenu")
P.loadAlong = { "ControlsMenu", "AudioMenu", "GraphicsMenu" }

function P.SettingsGameplayButton_clicked(e)
    showMenuSheet("GameplayMenu", true)
end

function P.SettingsMultiplayerOptionsButton_clicked(e)
    showMenuSheet("MultiplayerOptionsMenu", true)
end

function P.SettingsControlsButton_clicked(e)
    showMenuSheet("ControlsMenu", true)
end

function P.SettingsGraphicsButton_clicked(e)
    showMenuSheet("GraphicsMenu", true)
end

function P.SettingsAudioButton_clicked(e)
    showMenuSheet("AudioMenu", true)
end

function P.SettingsResetSettingsButton_clicked(e)
    -- reset settings
    logMessage(0, "event: reset settings")
end

function P.SettingsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

