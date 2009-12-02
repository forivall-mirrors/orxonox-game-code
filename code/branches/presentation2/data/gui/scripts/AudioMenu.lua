-- AudioMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    AudioMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "AudioMenu"
P.layoutString = "AudioMenu.layout"

function P:init()
    soundMgr = orxonox.SoundManager:getInstance()
    masterscrollbar_active = false
    musicscrollbar_active = false
    effectsscrollbar_active = false
    mastervolume = soundMgr:getVolume(orxonox.SoundType.none)
    musicvolume = soundMgr:getVolume(orxonox.SoundType.ambient)
    effectsvolume = soundMgr:getVolume(orxonox.SoundType.effects)
    window = tolua.cast(winMgr:getWindow("orxonox/MasterScrollbar"),"CEGUI::Scrollbar")
    window:setScrollPosition(mastervolume)
    window = tolua.cast(winMgr:getWindow("orxonox/MusicScrollbar"),"CEGUI::Scrollbar")
    window:setScrollPosition(musicvolume)
    window = tolua.cast(winMgr:getWindow("orxonox/EffectsScrollbar"),"CEGUI::Scrollbar")
    window:setScrollPosition(effectsvolume)
    dropdown = winMgr:getWindow("orxonox/AudioThemeCombobox")
    local themeList = {}
    table.insert(themeList, "Default")
    table.insert(themeList, "Drum n' Bass")
    for k,v in pairs(themeList) do
        item = CEGUI.createListboxTextItem(v)        
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toCombobox(dropdown):addItem(item)
    end
end

function P.AudioMasterScrollbar_changed(e)
    if masterscrollbar_active == false then
        window = tolua.cast(winMgr:getWindow("orxonox/MasterScrollbar"),"CEGUI::Scrollbar")
        volume = window:getScrollPosition()
        orxonox.CommandExecutor:execute("config SoundManager soundVolume_ " .. volume)
    end
end

function P.AudioMasterScrollbar_started(e)
    masterscrollbar_active = true
end

function P.AudioMasterScrollbar_ended(e)
    window = tolua.cast(winMgr:getWindow("orxonox/MasterScrollbar"),"CEGUI::Scrollbar")
    volume = window:getScrollPosition()
    orxonox.CommandExecutor:execute("config SoundManager soundVolume_ " .. volume)
    masterscrollbar_active = false
end

function P.AudioMusicScrollbar_changed(e)
    if musicscrollbar_active == false then
        window = tolua.cast(winMgr:getWindow("orxonox/MusicScrollbar"),"CEGUI::Scrollbar")
        volume = window:getScrollPosition()
        orxonox.CommandExecutor:execute("config SoundManager ambientVolume_ " .. volume)
    end
end

function P.AudioMusicScrollbar_started(e)
    musicscrollbar_active = true
end

function P.AudioMusicScrollbar_ended(e)
    window = tolua.cast(winMgr:getWindow("orxonox/MusicScrollbar"),"CEGUI::Scrollbar")
    volume = window:getScrollPosition()
    orxonox.CommandExecutor:execute("config SoundManager ambientVolume_ " .. volume)
    musicscrollbar_active = false
end

function P.AudioEffectsScrollbar_changed(e)
    if effectsscrollbar_active == false then
        window = tolua.cast(winMgr:getWindow("orxonox/EffectsScrollbar"),"CEGUI::Scrollbar")
        volume = window:getScrollPosition()
        orxonox.CommandExecutor:execute("config SoundManager effectsVolume_ " .. volume)
    end
end

function P.AudioEffectsScrollbar_started(e)
    effectsscrollbar_active = true
end

function P.AudioEffectsScrollbar_ended(e)
    window = tolua.cast(winMgr:getWindow("orxonox/EffectsScrollbar"),"CEGUI::Scrollbar")
    volume = window:getScrollPosition()
    orxonox.CommandExecutor:execute("config SoundManager effectsVolume_ " .. volume)
    effectsscrollbar_active = false
end

function P.AudioMuteMasterCheckbox_clicked(e)
--    if
--        mastervolume = soundMgr:getVolume(orxonox.SoundType.none)
--        window = tolua.cast(winMgr:getWindow("orxonox/MasterScrollbar"),"CEGUI::Scrollbar")
--        window:setScrollPosition(0)
--    end
    soundMgr:toggleMute(orxonox.SoundType.none)
end

function P.AudioMuteMusicCheckbox_clicked(e)
    soundMgr:toggleMute(orxonox.SoundType.ambient)
end

function P.AudioMuteEffectsCheckbox_clicked(e)
    soundMgr:toggleMute(orxonox.SoundType.effects)
end

function P.AudioThemeCombobox_changed(e)
--    local choice = winMgr:getWindow("orxonox/AudioThemeCombobox"):getFirstSelectedItem()
--    if choice == "Default" then
--        orxonox.CommandExecutor:execute("setMood default")
--        debug("default selected")
--    end
--    if choice == "Drum 'n Bass" then
--        orxonox.CommandExecutor:execute("setMood dnb")
--        debug("dnb selected")
--    end
end

function P.AudioBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

