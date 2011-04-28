-- AudioMenu.lua

local P = createMenuSheet("AudioMenu")

function P.onLoad()
    soundMgr = orxonox.SoundManager:getInstance()
    block = false
    masterscrollbar_active = false
    musicscrollbar_active = false
    effectsscrollbar_active = false
    mastervolume = soundMgr:getVolume(orxonox.SoundType.All)
    musicvolume = soundMgr:getVolume(orxonox.SoundType.Music)
    effectsvolume = soundMgr:getVolume(orxonox.SoundType.Effects)
    mastermute = soundMgr:getMute(orxonox.SoundType.All)
    musicmute = soundMgr:getMute(orxonox.SoundType.Music)
    effectsmute = soundMgr:getMute(orxonox.SoundType.Effects)
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
    listboxwindow = winMgr:getWindow("orxonox/AudioThemeListbox")
    local themeList = {}
    table.insert(themeList, "Default")
    table.insert(themeList, "Drum n' Bass")
    table.insert(themeList, "8-Bit Style")
    table.insert(themeList, "Corny Jazz")
    for k,v in pairs(themeList) do
        item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(listboxwindow):addItem(item)
    end
    if orxonox.getConfig("MoodManager", "mood_") == "dnb" then
        listboxwindow:setItemSelectState(1,true)
    elseif orxonox.getConfig("MoodManager", "mood_") == "eightbit" then
        listboxwindow:setItemSelectState(2,true)
    elseif orxonox.getConfig("MoodManager", "mood_") == "jazzy" then
        listboxwindow:setItemSelectState(3,true)
    else
        listboxwindow:setItemSelectState(0,true)
    end

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/AudioBackButton"),
            ["callback"]  = P.AudioBackButton_clicked
    })
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
        orxonox.config("SoundManager", "soundVolume_", mastervolume)
    end
end

function P.AudioMasterScrollbar_started(e)
    masterscrollbar_active = true
end

function P.AudioMasterScrollbar_ended(e)
    mastervolume = masterscrollbarwindow:getScrollPosition()
    orxonox.config("SoundManager", "soundVolume_", mastervolume)
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
        orxonox.config("SoundManager", "ambientVolume_", musicvolume)
    end
end

function P.AudioMusicScrollbar_started(e)
    musicscrollbar_active = true
end

function P.AudioMusicScrollbar_ended(e)
    musicmutewindow:setSelected(false)
    musicvolume = musicscrollbarwindow:getScrollPosition()
    orxonox.config("SoundManager", "ambientVolume_", musicvolume)
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
        orxonox.config("SoundManager", "effectsVolume_", effectsvolume)
    end
end

function P.AudioEffectsScrollbar_started(e)
    effectsscrollbar_active = true
end

function P.AudioEffectsScrollbar_ended(e)
    effectsmutewindow:setSelected(false)
    effectsvolume = effectsscrollbarwindow:getScrollPosition()
    orxonox.config("SoundManager", "effectsVolume_", effectsvolume)
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
    soundMgr:toggleMute(orxonox.SoundType.All)
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
    soundMgr:toggleMute(orxonox.SoundType.Music)
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
    soundMgr:toggleMute(orxonox.SoundType.Effects)
end

function P.AudioThemeListbox_changed(e)
    if listboxwindow:isItemSelected(1) then
        orxonox.config("MoodManager", "mood_", "dnb")
    elseif listboxwindow:isItemSelected(2) then
        orxonox.config("MoodManager", "mood_", "eightbit")
    elseif listboxwindow:isItemSelected(3) then
        orxonox.config("MoodManager", "mood_", "jazzy")
    else
        orxonox.config("MoodManager", "mood_", "default")
    end
end

function P.AudioBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

