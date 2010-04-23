-- AudioMenu.lua

local P = createMenuSheet("AudioMenu")

function P.onLoad()
    P.soundMgr = orxonox.SoundManager:getInstance()
    P.block = false
    P.masterscrollbar_active = false
    P.musicscrollbar_active = false
    P.effectsscrollbar_active = false
    P.mastervolume = P.soundMgr:getVolume(orxonox.SoundType.All)
    P.musicvolume = P.soundMgr:getVolume(orxonox.SoundType.Music)
    P.effectsvolume = P.soundMgr:getVolume(orxonox.SoundType.Effects)
    P.mastermute = P.soundMgr:getMute(orxonox.SoundType.All)
    P.musicmute = P.soundMgr:getMute(orxonox.SoundType.Music)
    P.effectsmute = P.soundMgr:getMute(orxonox.SoundType.Effects)
    P.masterscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MasterScrollbar"),"CEGUI::Scrollbar")
    P.musicscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/MusicScrollbar"),"CEGUI::Scrollbar")
    P.effectsscrollbarwindow = tolua.cast(winMgr:getWindow("orxonox/EffectsScrollbar"),"CEGUI::Scrollbar")
    P.mastermutewindow = tolua.cast(winMgr:getWindow("orxonox/MasterCheckbox"),"CEGUI::Checkbox")
    P.musicmutewindow = tolua.cast(winMgr:getWindow("orxonox/MusicCheckbox"),"CEGUI::Checkbox")
    P.effectsmutewindow = tolua.cast(winMgr:getWindow("orxonox/EffectsCheckbox"),"CEGUI::Checkbox")
    P.masterscrollbarwindow:setScrollPosition(P.mastervolume)
    P.musicscrollbarwindow:setScrollPosition(P.musicvolume)
    P.effectsscrollbarwindow:setScrollPosition(P.effectsvolume)
    P.mastermutewindow:setSelected(P.mastermute)
    P.musicmutewindow:setSelected(P.musicmute)
    P.effectsmutewindow:setSelected(P.effectsmute)
    local choice = "Default"
    P.listboxwindow = winMgr:getWindow("orxonox/AudioThemeListbox")
    local themeList = {}
    table.insert(themeList, "Default")
    table.insert(themeList, "Drum n' Bass")
    for k,v in pairs(themeList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(P.listboxwindow):addItem(item)
    end
    if orxonox.getConfig("MoodManager", "mood_") == "dnb" then
        P.listboxwindow:setItemSelectState(1,true)
    else
        P.listboxwindow:setItemSelectState(0,true)
    end
end

function P.AudioMasterScrollbar_changed(e)
    if P.mastermute then
        P.block = true
        P.mastermutewindow:setSelected(false)
        P.block = false
        P.mastermute = false
    end
    if P.masterscrollbar_active == false then
        P.mastervolume = P.masterscrollbarwindow:getScrollPosition()
        orxonox.config("SoundManager", "soundVolume_", P.mastervolume)
    end
end

function P.AudioMasterScrollbar_started(e)
    P.masterscrollbar_active = true
end

function P.AudioMasterScrollbar_ended(e)
    P.mastervolume = P.masterscrollbarwindow:getScrollPosition()
    orxonox.config("SoundManager", "soundVolume_", P.mastervolume)
    P.masterscrollbar_active = false
end

function P.AudioMusicScrollbar_changed(e)
    if P.musicmute then
        P.block = true
        P.musicmutewindow:setSelected(false)
        P.block = false
        P.musicmute = false
    end
    if P.musicscrollbar_active == false then
        P.musicvolume = P.musicscrollbarwindow:getScrollPosition()
        orxonox.config("SoundManager", "ambientVolume_", P.musicvolume)
    end
end

function P.AudioMusicScrollbar_started(e)
    P.musicscrollbar_active = true
end

function P.AudioMusicScrollbar_ended(e)
    P.musicmutewindow:setSelected(false)
    P.musicvolume = P.musicscrollbarwindow:getScrollPosition()
    orxonox.config("SoundManager", "ambientVolume_", P.musicvolume)
    P.musicscrollbar_active = false
end

function P.AudioEffectsScrollbar_changed(e)
    if P.effectsmute then
        P.block = true
        P.effectsmutewindow:setSelected(false)
        P.block = false
        P.effectsmute = false
    end
    if P.effectsscrollbar_active == false then
        P.effectsvolume = P.effectsscrollbarwindow:getScrollPosition()
        orxonox.config("SoundManager", "effectsVolume_", P.effectsvolume)
    end
end

function P.AudioEffectsScrollbar_started(e)
    P.effectsscrollbar_active = true
end

function P.AudioEffectsScrollbar_ended(e)
    P.effectsmutewindow:setSelected(false)
    P.effectsvolume = P.effectsscrollbarwindow:getScrollPosition()
    orxonox.config("SoundManager", "effectsVolume_", P.effectsvolume)
    P.effectsscrollbar_active = false
end

function P.AudioMuteMasterCheckbox_clicked(e)
    if P.block == false then
        if P.mastermute then
            P.masterscrollbarwindow:setScrollPosition(P.mastervolume)
            P.mastermute = false
        else
            temp = P.masterscrollbarwindow:getScrollPosition()
            P.masterscrollbarwindow:setScrollPosition(0)
            P.mastervolume = temp
            P.mastermute = true
        end
    end
    P.soundMgr:toggleMute(orxonox.SoundType.All)
end

function P.AudioMuteMusicCheckbox_clicked(e)
    if P.block == false then
        if P.musicmute then
            P.musicscrollbarwindow:setScrollPosition(P.musicvolume)
            P.musicmute = false
        else
            temp = P.musicscrollbarwindow:getScrollPosition()
            P.musicscrollbarwindow:setScrollPosition(0)
            P.musicvolume = temp
            P.musicmute = true
        end
    end
    P.soundMgr:toggleMute(orxonox.SoundType.Music)
end

function P.AudioMuteEffectsCheckbox_clicked(e)
    if P.block == false then
        if P.effectsmute then
            P.effectsscrollbarwindow:setScrollPosition(P.effectsvolume)
            P.effectsmute = false
        else
            temp = P.effectsscrollbarwindow:getScrollPosition()
            P.effectsscrollbarwindow:setScrollPosition(0)
            P.effectsvolume = temp
            P.effectsmute = true
        end
    end
    P.soundMgr:toggleMute(orxonox.SoundType.Effects)
end

function P.AudioThemeListbox_changed(e)
    if P.listboxwindow:isItemSelected(1) then
        orxonox.config("MoodManager", "mood_", "dnb")
    else
        orxonox.config("MoodManager", "mood_", "default")
    end
end

function P.AudioBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

