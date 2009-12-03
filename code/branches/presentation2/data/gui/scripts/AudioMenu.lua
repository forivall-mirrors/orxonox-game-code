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
    block = false
    masterscrollbar_active = false
    musicscrollbar_active = false
    effectsscrollbar_active = false
    mastervolume = soundMgr:getVolume(orxonox.SoundType.none)
    musicvolume = soundMgr:getVolume(orxonox.SoundType.ambient)
    effectsvolume = soundMgr:getVolume(orxonox.SoundType.effects)
    mastermute = soundMgr:getMute(orxonox.SoundType.none)
    musicmute = soundMgr:getMute(orxonox.SoundType.ambient)
    effectsmute = soundMgr:getMute(orxonox.SoundType.effects)
    masterscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MasterScrollbar"),"CEGUI::Scrollbar")
    musicscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MusicScrollbar"),"CEGUI::Scrollbar")
    effectsscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/EffectsScrollbar"),"CEGUI::Scrollbar")
    mastermutewindow = tolua.cast(winMgr:getWindow("orxonox/MasterCheckbox"),"CEGUI::Checkbox")
    musicmutewindow = tolua.cast(winMgr:getWindow("orxonox/MusicCheckbox"),"CEGUI::Checkbox")
    effectsmutewindow = tolua.cast(winMgr:getWindow("orxonox/EffectsCheckbox"),"CEGUI::Checkbox")
    masterscrollbarwindow:setScrollPosition(mastervolume)
    musicscrollbarwindow:setScrollPosition(musicvolume)
    effectsscrollbarwindow:setScrollPosition(effectsvolume)
    mastermutewindow:setSelected(mastermute)
    musicmutewindow:setSelected(musicmute)
    effectsmutewindow:setSelected(effectsmute)
    choice = "Default"
    dropdownwindow = winMgr:getWindow("orxonox/AudioThemeCombobox")
    local themeList = {}
    table.insert(themeList, "Default")
    table.insert(themeList, "Drum n' Bass")
    for k,v in pairs(themeList) do
        item = CEGUI.createListboxTextItem(v)        
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toCombobox(dropdownwindow):addItem(item)
    end
    dropdownwindow:setItemSelectState(0,true)
end

function P.AudioMasterScrollbar_changed(e)
    if mastermute then
        block = true
        mastermutewindow:setSelected(false)
        block = false
        mastermute = false
    end
    if masterscrollbar_active == false then
        mastervolume = masterscrollbarwindow:getScrollPosition()
        orxonox.CommandExecutor:execute("config SoundManager soundVolume_ " .. mastervolume)
    end
end

function P.AudioMasterScrollbar_started(e)
    masterscrollbar_active = true
end

function P.AudioMasterScrollbar_ended(e)
    mastervolume = masterscrollbarwindow:getScrollPosition()
    orxonox.CommandExecutor:execute("config SoundManager soundVolume_ " .. mastervolume)
    masterscrollbar_active = false
end

function P.AudioMusicScrollbar_changed(e)
    if musicmute then
        block = true
        musicmutewindow:setSelected(false)
        block = false
        musicmute = false
    end
    if musicscrollbar_active == false then
        musicvolume = musicscrollbarwindow:getScrollPosition()
        orxonox.CommandExecutor:execute("config SoundManager ambientVolume_ " .. musicvolume)
    end
end

function P.AudioMusicScrollbar_started(e)
    musicscrollbar_active = true
end

function P.AudioMusicScrollbar_ended(e)
    musicmutewindow:setSelected(false)
    musicvolume = musicscrollbarwindow:getScrollPosition()
    orxonox.CommandExecutor:execute("config SoundManager ambientVolume_ " .. musicvolume)
    musicscrollbar_active = false
end

function P.AudioEffectsScrollbar_changed(e)
    if effectsmute then
        block = true
        effectsmutewindow:setSelected(false)
        block = false
        effectsmute = false
    end
    if effectsscrollbar_active == false then
        effectsvolume = effectsscrollbarwindow:getScrollPosition()
        orxonox.CommandExecutor:execute("config SoundManager effectsVolume_ " .. effectsvolume)
    end
end

function P.AudioEffectsScrollbar_started(e)
    effectsscrollbar_active = true
end

function P.AudioEffectsScrollbar_ended(e)
    effectsmutewindow:setSelected(false)
    effectsvolume = effectsscrollbarwindow:getScrollPosition()
    orxonox.CommandExecutor:execute("config SoundManager effectsVolume_ " .. effectsvolume)
    effectsscrollbar_active = false
end

function P.AudioMuteMasterCheckbox_clicked(e)
    if block == false then
        if mastermute then
            masterscrollbarwindow:setScrollPosition(mastervolume)
            mastermute = false
        else
            temp = masterscrollbarwindow:getScrollPosition()
            masterscrollbarwindow:setScrollPosition(0)
            mastervolume = temp
            mastermute = true
        end
    end
    soundMgr:toggleMute(orxonox.SoundType.none)
end

function P.AudioMuteMusicCheckbox_clicked(e)
    if block == false then
        if musicmute then
            musicscrollbarwindow:setScrollPosition(musicvolume)
            musicmute = false
        else
            temp = musicscrollbarwindow:getScrollPosition()
            musicscrollbarwindow:setScrollPosition(0)
            musicvolume = temp
            musicmute = true
        end
    end
    soundMgr:toggleMute(orxonox.SoundType.ambient)
end

function P.AudioMuteEffectsCheckbox_clicked(e)
    if block == false then
        if effectsmute then
            effectsscrollbarwindow:setScrollPosition(effectsvolume)
            effectsmute = false
        else
            temp = effectsscrollbarwindow:getScrollPosition()
            effectsscrollbarwindow:setScrollPosition(0)
            effectsvolume = temp
            effectsmute = true
        end
    end
    soundMgr:toggleMute(orxonox.SoundType.effects)
end

function P.AudioThemeCombobox_changed(e)
    if dropdownwindow:isItemSelected(1) then
        orxonox.CommandExecutor:execute("setMood dnb")
    else
        orxonox.CommandExecutor:execute("setMood default")
    end
end

function P.AudioBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

