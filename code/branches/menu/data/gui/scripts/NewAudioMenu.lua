-- NewAudioMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    NewAudioMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "NewAudioMenu"
P.layoutString = "NewAudioMenu.layout"

function P.AudioMusicScrollbar_clicked(e)
    -- music volume
    debug("event: music volume")
end

function P.AudioSoundScrollbar_clicked(e)
    -- sound volume
    debug("event: sound volume")
end

function P.AudioMuteMusicCheckbox_clicked(e)
    -- mute music
    debug("event: mute music")
end

function P.AudioMuteSoundCheckbox_clicked(e)
    -- mute sound
    debug("event: mute sound")
end

function P.AudioBackButton_clicked(e)
    hideGUI("NewAudioMenu")
    debug("event: back")
end

return P

