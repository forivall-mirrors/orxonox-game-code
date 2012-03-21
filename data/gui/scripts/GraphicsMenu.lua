-- GraphicsMenu.lua

local P = createMenuSheet("GraphicsMenu")

P.resolutionList = {"custom", "640 x 480", "720 x 480", "720 x 576", "800 x 600", "1024 x 600", "1024 x 768", "1152 x 864", "1280 x 720", "1280 x 800", "1280 x 960", "1280 x 1024", "1360 x 768", "1440 x 900", "1600 x 900", "1600 x 1200", "1680 x 1050"}
P.schemeList = {"TaharezGreen", "Orxonox"}
P.fsaaList = {"0", "2", "4", "8", "8 [Quality]"}
P.particleLodList = {"None", "Low", "Normal", "High"}

function P:onLoad()
    -------------------
    -- Button matrix --
    -------------------

    P:setButton(1, 1, {
            ["button"] = winMgr:getWindow("orxonox/GraphicsOkButton"),
            ["callback"]  = P.callback_Ok_Clicked
    })

    P:setButton(1, 2, {
            ["button"] = winMgr:getWindow("orxonox/GraphicsCancelButton"),
            ["callback"]  = P.callback_Cancel_Clicked
    })

    -- place apply button at the bottom in the matrix, even though it's in fact at the top, to make the OK button highlighted by default
    P:setButton(2, 1, {
            ["button"] = winMgr:getWindow("orxonox/Display/Resolution/Apply"),
            ["callback"]  = P.callback_Apply_Clicked
    })

    -----------------
    -- Combo boxes --
    -----------------

    -- resolution combobox
    local resolutionCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/Resolution/Combobox"))
    resolutionCombobox:setReadOnly(true)

    for k,v in pairs(P.resolutionList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        resolutionCombobox:addItem(item)
    end

    -- themes combobox
    local themeCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/Theme/Combobox"))
    themeCombobox:setReadOnly(true)

    for k,v in pairs(P.schemeList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        themeCombobox:addItem(item)
    end

    -- fsaa combobox
    local fsaaCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/More/FSAA"))
    fsaaCombobox:setReadOnly(true)

    for k,v in pairs(P.fsaaList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        fsaaCombobox:addItem(item)
    end

    -- particle lod combobox
    local particleLodCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Settings/ParticleLodCombobox"))
    particleLodCombobox:setReadOnly(true)

    for k,v in pairs(P.particleLodList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        particleLodCombobox:addItem(item)
    end
end

function P:onShow()
    -----------------
    -- Display tab --
    -----------------

    -- fullscreen checkbox / resolution combobox / resolution editboxes
    self:onWindowResized()

    -- apply button
    self.updateApplyButton()

    -- aspect ratio editbox
    local aspectRatioEditbox = winMgr:getWindow("orxonox/Display/Resolution/AspectRatio")
    local currentAspectRatio = orxonox.CommandExecutor:query("getConfig Camera aspectRatio_")
    aspectRatioEditbox:setText(currentAspectRatio)

    -- themes combobox
    local themeCombobox =  CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/Theme/Combobox"))
    local currentTheme = orxonox.CommandExecutor:query("getConfig GUIManager guiScheme_")

    for i = 0, themeCombobox:getDropList():getItemCount() - 1 do
        local item = themeCombobox:getListboxItemFromIndex(i)
        themeCombobox:setItemSelectState(item, (item:getText() == currentTheme))
    end

    -- vsync checkbox
    local vsyncCheckbox = winMgr:getWindow("orxonox/Display/More/VSync")
    local hasVSync = orxonox.GraphicsManager:getInstance():hasVSyncEnabled()
    CEGUI.toCheckbox(vsyncCheckbox):setSelected(hasVSync)

    -- fsaa combobox
    local fsaaCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/More/FSAA"))
    local currentFSAAMode = orxonox.GraphicsManager:getInstance():getFSAAMode()

    for i = 0, fsaaCombobox:getDropList():getItemCount() - 1 do
        local item = fsaaCombobox:getListboxItemFromIndex(i)
        fsaaCombobox:setItemSelectState(item, (item:getText() == currentFSAAMode))
    end

    -- notice
    self:updateRedLabel()

    ------------------
    -- Settings tab --
    ------------------

    -- fov editbox
    local fovEditbox = winMgr:getWindow("orxonox/Settings/Fov")
    local currentFov = orxonox.CommandExecutor:query("getConfig Camera fov_")
    fovEditbox:setText(currentFov)

    -- fps limit editbox
    local fpsEditbox = winMgr:getWindow("orxonox/Settings/FpsLimit")
    local currentFpsLimit = orxonox.CommandExecutor:query("getConfig GraphicsSettings fpsLimit")
    fpsEditbox:setText(currentFpsLimit)

    -- particle lod combobox
    local particleLodCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Settings/ParticleLodCombobox"))
    local currentParticleLod = orxonox.CommandExecutor:query("getConfig GraphicsSettings particlesDetailLevel")

    if currentParticleLod == "" then
        particleLodCombobox:disable()
    else
        particleLodCombobox:enable()

        for i = 0, particleLodCombobox:getDropList():getItemCount() - 1 do
            local item = particleLodCombobox:getListboxItemFromIndex(i)
            particleLodCombobox:setItemSelectState(item, (tostring(i) == currentParticleLod))
        end
    end

    -- mesh lod checkbox
    local meshLodCheckbox = winMgr:getWindow("orxonox/Settings/MeshLodCheckbox")
    local hasMeshLod = orxonox.CommandExecutor:query("getConfig GraphicsSettings enableMeshLoD")
    if hasMeshLod == "true" then
        hasMeshLod = true
    elseif hasMeshLod == "false" then
        hasMeshLod = false
    end
    CEGUI.toCheckbox(meshLodCheckbox):setSelected(hasMeshLod)

    -- motion blur checkbox
    local motionBlurCheckbox = winMgr:getWindow("orxonox/Settings/MotionBlurCheckbox")
    local hasMotionBlur = orxonox.CommandExecutor:query("getConfig GraphicsSettings enableMotionBlur")
    if hasMotionBlur == "true" then
        hasMotionBlur = true
    elseif hasMotionBlur == "false" then
        hasMotionBlur = false
    end
    CEGUI.toCheckbox(motionBlurCheckbox):setSelected(hasMotionBlur)
end

function P:onWindowResized()
    -- fullscreen checkbox
    local fullscreenCheckbox = winMgr:getWindow("orxonox/Display/Resolution/Fullscreen")
    local isFullscreen = orxonox.GraphicsManager:getInstance():isFullScreen()
    CEGUI.toCheckbox(fullscreenCheckbox):setSelected(isFullscreen)

    -- resolution combobox
    local resolutionCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/Resolution/Combobox"))

    local currentWidth = orxonox.GraphicsManager:getInstance():getWindowWidth()
    local currentHeight = orxonox.GraphicsManager:getInstance():getWindowHeight()

    if P.forceResolutionEditboxes then
        currentWidth = P.newWidth
        currentHeight = P.newHeight
        P.forceResolutionEditboxes = false
    end

    local currentResolution = currentWidth .. " x " .. currentHeight

    for i = 0, resolutionCombobox:getDropList():getItemCount() - 1 do
        local item = resolutionCombobox:getListboxItemFromIndex(i)
        resolutionCombobox:setItemSelectState(item, item:getText() == currentResolution)
    end

    -- resolution editboxes
    self.updateResolutionEditboxes()
end

----------------------
-- Helper functions --
----------------------

-- updates the text of the resolution checkboxes and checks if they should be enabled (only if the "custom" resolution was selected)
function P.updateResolutionEditboxes()
    -- resolution combobox
    local resolutionCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Display/Resolution/Combobox"))

    local currentWidth = orxonox.GraphicsManager:getInstance():getWindowWidth()
    local currentHeight = orxonox.GraphicsManager:getInstance():getWindowHeight()

    -- resolution editboxes
    local widthEditbox = winMgr:getWindow("orxonox/Display/Resolution/EditboxWidth")
    local heightEditbox = winMgr:getWindow("orxonox/Display/Resolution/EditboxHeight")
    widthEditbox:disable()
    heightEditbox:disable()

    -- selected combobox item
    local item = resolutionCombobox:getSelectedItem()
    if item then
        local itemText = item:getText()
        if itemText ~= "custom" then
            currentWidth = string.sub(itemText, 1, string.find(itemText, "x") - 2)
            currentHeight = string.sub(itemText, string.find(itemText, "x") + 2)
        else
            widthEditbox:enable()
            heightEditbox:enable()
        end
    end

    widthEditbox:setText(currentWidth)
    heightEditbox:setText(currentHeight)
end

-- checks if the apply button should be enabled or disabled (only enabled if the current settings are different from the selected values)
function P.updateApplyButton()
    -- fullscreen checkbox
    local fullscreenCheckbox = winMgr:getWindow("orxonox/Display/Resolution/Fullscreen")
    local isFullscreen = orxonox.GraphicsManager:getInstance():isFullScreen()
    local fullscreenChanged = (isFullscreen ~= CEGUI.toCheckbox(fullscreenCheckbox):isSelected())

    -- resolution editboxes
    local widthEditbox = winMgr:getWindow("orxonox/Display/Resolution/EditboxWidth")
    local heightEditbox = winMgr:getWindow("orxonox/Display/Resolution/EditboxHeight")
    local currentWidth = tostring(orxonox.GraphicsManager:getInstance():getWindowWidth())
    local currentHeight = tostring(orxonox.GraphicsManager:getInstance():getWindowHeight())
    local widthChanged = (currentWidth ~= widthEditbox:getText())
    local heightChanged = (currentHeight ~= heightEditbox:getText())
    local resolutionEditboxesEnabled = not widthEditbox:isDisabled()

    -- apply button
    local applyButton = winMgr:getWindow("orxonox/Display/Resolution/Apply")

    if fullscreenChanged or widthChanged or heightChanged or resolutionEditboxesEnabled then
        applyButton:enable()
    else
        applyButton:disable()
    end
end

function P.updateRedLabel()
    -- theme
    local themeCombobox = winMgr:getWindow("orxonox/Display/Theme/Combobox")
    local currentTheme = orxonox.CommandExecutor:query("getConfig GUIManager guiScheme_")
    local themeChanged = (currentTheme ~= themeCombobox:getText())

    -- vsync
    local vsyncCheckbox = winMgr:getWindow("orxonox/Display/More/VSync")
    local hasVSync = orxonox.GraphicsManager:getInstance():hasVSyncEnabled()
    local vsyncChanged = (hasVSync ~= CEGUI.toCheckbox(vsyncCheckbox):isSelected())

    -- fsaa
    local fsaaCombobox = winMgr:getWindow("orxonox/Display/More/FSAA")
    local currentFSAAMode = orxonox.GraphicsManager:getInstance():getFSAAMode()
    local fsaaChanged = (currentFSAAMode ~= fsaaCombobox:getText())

    local needRestart = themeChanged or vsyncChanged or fsaaChanged

    local notice = winMgr:getWindow("orxonox/Display/Notice")
    notice:setVisible(not needRestart)
    local noticeRed = winMgr:getWindow("orxonox/Display/NoticeRed")
    noticeRed:setVisible(needRestart)
end

---------------------
-- Event callbacks --
---------------------

-- resolution

function P.callback_FullscreenCheckbox_CheckStateChanged(e)
    P.updateApplyButton()
end

function P.callback_ResolutionCombobox_ListSelectionAccepted(e)
    P.updateResolutionEditboxes()
end

function P.callback_ResolutionEditboxWidth_TextChanged(e)
    P.updateApplyButton()
end

function P.callback_ResolutionEditboxHeight_TextChanged(e)
    P.updateApplyButton()
end

-- theme

function P.callback_ThemeCombobox_ListSelectionAccepted(e)
    P.updateRedLabel()
end

-- vsync

function P.callback_VSyncCheckbox_CheckStateChanged(e)
    P.updateRedLabel()
end

-- fsaa

function P.callback_FSAACombobox_ListSelectionAccepted(e)
    P.updateRedLabel()
end

-- buttons

function P.callback_Apply_Clicked(e)
    -- resolution
    local fullscreenCheckbox = winMgr:getWindow("orxonox/Display/Resolution/Fullscreen")
    local checkedFullscreen = tostring(CEGUI.toCheckbox(fullscreenCheckbox):isSelected())

    local widthEditbox = winMgr:getWindow("orxonox/Display/Resolution/EditboxWidth")
    local heightEditbox = winMgr:getWindow("orxonox/Display/Resolution/EditboxHeight")

    P.newWidth = widthEditbox:getText()
    P.newHeight = heightEditbox:getText()
    P.forceResolutionEditboxes = true

    -- start revert timer
    P.oldWidth = orxonox.GraphicsManager:getInstance():getWindowWidth()
    P.oldHeight = orxonox.GraphicsManager:getInstance():getWindowHeight()
    P.oldFullscreen = orxonox.GraphicsManager:getInstance():isFullScreen()

    P.revertTimerHandle = orxonox.CommandExecutor:query("delayreal 10 {hideGUI DecisionPopup; GraphicsManager setScreenResolution " .. P.oldWidth .. " " .. P.oldHeight .. " " .. tostring(P.oldFullscreen) .. "; config Core lastLevelTimestamp_ [expr [getConfig Core ogreConfigTimestamp_] + 1]}")

    -- change settings
    orxonox.CommandExecutor:execute("GraphicsManager setScreenResolution " .. P.newWidth .. " " .. P.newHeight .. " " .. checkedFullscreen)

    P.updateApplyButton()

    -- prompt for confirmation
    openDecisionPopup("Do you want to keep these settings? (Settings will be reverted in 10 seconds if not accepted)", GraphicsMenu.callback_ApplyDecisionPopup)
    if checkedFullscreen then
        showCursor()
    end
end

function P.callback_ApplyDecisionPopup(pressedOK)
    orxonox.CommandExecutor:execute("killdelay " .. P.revertTimerHandle)

    if not pressedOK then
        orxonox.CommandExecutor:execute("GraphicsManager setScreenResolution " .. P.oldWidth .. " " .. P.oldHeight .. " " .. tostring(P.oldFullscreen))
        P:onShow()
    end

    -- update timestamp to avoid showing the ogre config dialog again after the user accepted or reverted the resolution
    orxonox.CommandExecutor:execute("config Core lastLevelTimestamp_ [expr [getConfig Core ogreConfigTimestamp_] + 1]")
end

function P.callback_Ok_Clicked(e)
    -- aspect ratio
    local aspectRatioEditbox = winMgr:getWindow("orxonox/Display/Resolution/AspectRatio")
    orxonox.CommandExecutor:execute("config Camera aspectRatio_ " .. aspectRatioEditbox:getText())

    -- theme
    local themeCombobox = winMgr:getWindow("orxonox/Display/Theme/Combobox")
    orxonox.CommandExecutor:execute("config GUIManager guiScheme_ " .. themeCombobox:getText())

    -- vsync
    local vsyncCheckbox = winMgr:getWindow("orxonox/Display/More/VSync")
    local hasVSync = orxonox.GraphicsManager:getInstance():hasVSyncEnabled()
    if hasVSync ~= CEGUI.toCheckbox(vsyncCheckbox):isSelected() then
        orxonox.CommandExecutor:execute("GraphicsManager setVSync " .. tostring(CEGUI.toCheckbox(vsyncCheckbox):isSelected()))
    end

    -- fsaa
    local fsaaCombobox = winMgr:getWindow("orxonox/Display/More/FSAA")
    local currentFSAAMode = orxonox.GraphicsManager:getInstance():getFSAAMode()
    if currentFSAAMode ~= fsaaCombobox:getText() then
        orxonox.CommandExecutor:execute("GraphicsManager setFSAA {" .. fsaaCombobox:getText() .. "}") -- enclose argument in { ... } because it can contain [brackets] (conflicts with tcl)
    end

    -- fov
    local fovEditbox = winMgr:getWindow("orxonox/Settings/Fov")
    orxonox.CommandExecutor:execute("config Camera fov_ " .. fovEditbox:getText())

    -- fps limit
    local fpsEditbox = winMgr:getWindow("orxonox/Settings/FpsLimit")
    orxonox.CommandExecutor:execute("config GraphicsSettings fpsLimit " .. fpsEditbox:getText())

    -- particle lod
    local particleLodCombobox = CEGUI.toCombobox(winMgr:getWindow("orxonox/Settings/ParticleLodCombobox"))
    local item = particleLodCombobox:getSelectedItem()
    if item then
        orxonox.CommandExecutor:execute("config GraphicsSettings particlesDetailLevel " .. particleLodCombobox:getItemIndex(item))
    end

    -- mesh lod
    local meshLodCheckbox = winMgr:getWindow("orxonox/Settings/MeshLodCheckbox")
    orxonox.CommandExecutor:execute("config GraphicsSettings enableMeshLoD " .. tostring(CEGUI.toCheckbox(meshLodCheckbox):isSelected()))

    -- motion blur
    local motionBlurCheckbox = winMgr:getWindow("orxonox/Settings/MotionBlurCheckbox")
    orxonox.CommandExecutor:execute("config GraphicsSettings enableMotionBlur " .. tostring(CEGUI.toCheckbox(motionBlurCheckbox):isSelected()))

    hideMenuSheet(P.name)
end

function P.callback_Cancel_Clicked(e)
    hideMenuSheet(P.name)
end

return P

