-- SettingsMenu.lua

local P = createMenuSheet("SettingsMenu")
P.loadAlong = { "ControlsMenu", "AudioMenu", "GraphicsMenu" }

function P.onLoad()
    --"Gameplay" and "Multiplayer Options" are not integrated in the list
    --buttons are arranged in a 4x2 matrix.
    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/GraphicsButton"),
            ["callback"]  = P.SettingsGraphicsButton_clicked
    })

    P:setButton(2, 2, {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/AudioButton"),
            ["callback"]  = P.SettingsAudioButton_clicked
    })

    P:setButton(3, 1, {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/ControlsButton"),
            ["callback"]  = P.SettingsControlsButton_clicked
    })

    P:setButton(3, 2, {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/MiscellaneousButton"),
            ["callback"]  = P.SettingsMiscellaneousButton_clicked
    })

    P:setButton(4, 1, {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/SettingsBackButton"),
            ["callback"]  = P.SettingsBackButton_clicked
    })

    P:setButton(4, 2, P:getButton(4, 1))
end

function P.onShow()
    local window = winMgr:getWindow("orxonox/SettingsMenu/AudioButton")
    if not orxonox.SoundManager:exists() then
        window:setProperty("Disabled", "true")
    else
        window:setProperty("Disabled", "false")
    end
end

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

function P.SettingsMiscellaneousButton_clicked(e)
    showMenuSheet("MiscConfigMenu", true)
end

function P.SettingsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

