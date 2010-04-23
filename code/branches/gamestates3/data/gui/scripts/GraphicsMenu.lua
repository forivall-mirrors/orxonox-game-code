-- GraphicsMenu.lua

local P = createMenuSheet("GraphicsMenu")

function P.onLoad()
    P.block = true
    P.file = orxonox.PathConfig:getConfigPathString() .. orxonox.getConfig("GraphicsManager", "ogreConfigFile_")
    P.search_mode = 0
    local f = io.open(P.file, "r")
    local firstline = f:read("*line")
    local rendersystem = string.sub(firstline, 15)
    for line in f:lines() do
        if P.search_mode == 0 then
            if string.find(line, rendersystem) ~= nil then
                P.search_mode = 1
            end
        end
        if P.search_mode == 1 then
            if string.sub(line, 1, 11) == "Full Screen" then
                if string.sub(line, 13) == "Yes" then
                    P.fullscreen = true
                else
                    P.fullscreen = false
                end
            end
            if string.sub(line, 1, 10) == "Video Mode" then
                if string.match(line, "@") == "@" then
                    P.resolution = string.sub(line, 12, string.find(line, "@")-2)
                else
                    P.resolution = string.sub(line, 12)
                end
                break
            end
        end
    end
    f:close()
    local fullscreenwindow = tolua.cast(winMgr:getWindow("orxonox/FullscreenCheckbox"),"CEGUI::Checkbox")
    fullscreenwindow:setSelected(P.fullscreen)
    P.listboxwindow = winMgr:getWindow("orxonox/ResolutionListbox")
    local resolutionList = {}
    table.insert(resolutionList, "640 x 480")
    table.insert(resolutionList, "720 x 480")
    table.insert(resolutionList, "720 x 576")
    table.insert(resolutionList, "800 x 480")
    table.insert(resolutionList, "800 x 600")
    table.insert(resolutionList, "1024 x 480")
    table.insert(resolutionList, "1024 x 600")
    table.insert(resolutionList, "1024 x 768")
    table.insert(resolutionList, "1152 x 864")
    table.insert(resolutionList, "1280 x 720")
    table.insert(resolutionList, "1280 x 768")
    table.insert(resolutionList, "1440 x 900")
    for k,v in pairs(resolutionList) do
        local item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        CEGUI.toListbox(P.listboxwindow):addItem(item)
    end
    if P.resolution == "640 x 480" then
        P.listboxwindow:setItemSelectState(0,true)
    elseif P.resolution == "720 x 480" then
        P.listboxwindow:setItemSelectState(1,true)
    elseif P.resolution == "720 x 576" then
        P.listboxwindow:setItemSelectState(2,true)
    elseif P.resolution == "800 x 480" then
        P.listboxwindow:setItemSelectState(3,true)
    elseif P.resolution == "800 x 600" then
        P.listboxwindow:setItemSelectState(4,true)
    elseif P.resolution == "1024 x 480" then
        P.listboxwindow:setItemSelectState(5,true)
    elseif P.resolution == "1024 x 600" then
        P.listboxwindow:setItemSelectState(6,true)
    elseif P.resolution == "1024 x 768" then
        P.listboxwindow:setItemSelectState(7,true)
    elseif P.resolution == "1152 x 864" then
        P.listboxwindow:setItemSelectState(8,true)
    elseif P.resolution == "1280 x 720" then
        P.listboxwindow:setItemSelectState(9,true)
    elseif P.resolution == "1280 x 768" then
        P.listboxwindow:setItemSelectState(10,true)
    elseif P.resolution == "1440 x 900" then
        P.listboxwindow:setItemSelectState(11,true)
    end
    P.scrollbar_active = false
    P.block = false
end

function P.GraphicsResolutionListbox_changed(e)
    if P.listboxwindow:isItemSelected(0) then
        P.resolution = "640 x 480"
    elseif P.listboxwindow:isItemSelected(1) then
        P.resolution = "720 x 480"
    elseif P.listboxwindow:isItemSelected(2) then
        P.resolution = "720 x 576"
    elseif P.listboxwindow:isItemSelected(3) then
        P.resolution = "800 x 480"
    elseif P.listboxwindow:isItemSelected(4) then
        P.resolution = "800 x 600"
    elseif P.listboxwindow:isItemSelected(5) then
        P.resolution = "1024 x 480"
    elseif P.listboxwindow:isItemSelected(6) then
        P.resolution = "1024 x 600"
    elseif P.listboxwindow:isItemSelected(7) then
        P.resolution = "1024 x 768"
    elseif P.listboxwindow:isItemSelected(8) then
        P.resolution = "1152 x 864"
    elseif P.listboxwindow:isItemSelected(9) then
        P.resolution = "1280 x 720"
    elseif P.listboxwindow:isItemSelected(10) then
        P.resolution = "1280 x 768"
    elseif P.listboxwindow:isItemSelected(11) then
        P.resolution = "1440 x 900"
    end
    P.search_mode = 0
    local f = io.open(P.file, "r")
    local firstline = f:read("*line")
    local text = firstline .. "\n"
    local rendersystem = string.sub(firstline, 15)
    for line in f:lines() do
        if P.search_mode == 0 then
            if string.find(line, rendersystem) ~= nil then
                P.search_mode = 1
            end
        end
        if P.search_mode == 1 then
            if string.sub(line, 1, 10) == "Video Mode" then
                if string.match(line, "@") == "@" then
                    line = "Video Mode=" .. P.resolution .. string.sub(line, string.find(line, "@")-1)
                else
                    line = "Video Mode=" .. P.resolution
                end
                P.search_mode = 2
            end
        end
        text = text .. line .. "\n"
    end
    f:close()
    f = io.open(P.file, "w")
    f:write(text)
    f:close()
end

function P.GraphicsBrightnessScrollbar_changed(e)
    if P.scrollbar_active == false then
        -- brightness
        logMessage(0, "event: brightness")
    end
end

function P.GraphicsBrightnessScrollbar_started(e)
    P.scrollbar_active = true
end

function P.GraphicsBrightnessScrollbar_ended(e)
    -- brightness
    logMessage(0, "event: brightness")
    P.scrollbar_active = false
end

function P.GraphicsFullscreenCheckbox_clicked(e)
    if P.block == false then
        P.search_mode = 0
        local f = io.open(P.file, "r")
        local firstline = f:read("*line")
        local text = firstline .. "\n"
        local rendersystem = string.sub(firstline, 15)
        for line in f:lines() do
            if P.search_mode == 0 then
                if string.find(line, rendersystem) ~= nil then
                    P.search_mode = 1
                end
            end
            if P.search_mode == 1 then
                if string.sub(line, 1, 11) == "Full Screen" then
                    if P.fullscreen == true then
                        line = "Full Screen=No"
                        P.fullscreen = false
                    else
                        line = "Full Screen=Yes"
                        P.fullscreen = true
                    end
                    P.search_mode = 2
                end
            end
            text = text .. line .. "\n"
        end
        f:close()
        f = io.open(P.file, "w")
        f:write(text)
        f:close()
    end
end

function P.GraphicsBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P

