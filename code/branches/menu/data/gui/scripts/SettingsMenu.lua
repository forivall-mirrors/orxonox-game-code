-- SettingsMenu.lua

local P = createMenuSheet("SettingsMenu")
P.loadAlong = { "ControlsMenu", "AudioMenu", "GraphicsMenu" }

P.buttonList = {}

function P.onLoad()
    --"Gameplay" and "Multiplayer Options" are not integrated in the list
    --buttons are arranged in a 4x2 matrix. The lower-right element is not in the matrix!
    local item = {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/GraphicsButton"),
            ["function"]  = P.SettingsGraphicsButton_clicked
    }
    P.buttonList[2] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/AudioButton"),
            ["function"]  = P.SettingsAudioButton_clicked
    }
    P.buttonList[4] = item


    local item = {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/ControlsButton"),
            ["function"]  = P.SettingsControlsButton_clicked
    }
    P.buttonList[5] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/MiscellaneousButton"),
            ["function"]  = P.SettingsMiscellaneousButton_clicked
    }
    P.buttonList[6] = item

    local item = {
            ["button"] = winMgr:getWindow("orxonox/SettingsMenu/SettingsBackButton"),
            ["function"]  = P.SettingsBackButton_clicked
    }
    P.buttonList[7] = item

end

function P.onShow()
    --indices to iterate through buttonlist
    P.oldindex = 3
    P.index = 2
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

function P.onKeyPressed() 
    buttonIteratorHelper(P.buttonList, code, P, 4, 2)
end

return P

