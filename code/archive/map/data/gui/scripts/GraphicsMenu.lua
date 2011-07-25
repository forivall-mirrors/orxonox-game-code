-- GraphicsMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    GraphicsMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "GraphicsMenu"
P.layoutString = "GraphicsMenu.layout"

function P:init()
    block = true
    file = orxonox.PathConfig:getConfigPathString() .. orxonox.getConfig("GraphicsManager", "ogreConfigFile_")
    search_mode = 0
    f = io.open(file, "r")
    firstline = f:read("*line")
    rendersystem = string.sub(firstline, 15)
    for line in f:lines() do
        if search_mode == 0 then
            if string.find(line, rendersystem) ~= nil then
                search_mode = 1
            end
        end
        if search_mode == 1 then
            if string.sub(line, 1, 11) == "Full Screen" then
                if string.sub(line, 13) == "Yes" then
                    fullscreen = true
                else
                    fullscreen = false
                end
            end
            if string.sub(line, 1, 10) == "Video Mode" then
                if string.match(line, "@") == "@" then
                    resolution = string.sub(line, 12, string.find(line, "@")-2)
                else
                    resolution = string.sub(line, 12)
                end
                break
            end
        end
    end
    f:close()
    local fullscreenwindow = tolua.cast(winMgr:getWindow("orxonox/FullscreenCheckbox"),"CEGUI::Checkbox")
    fullscreenwindow:setSelected(fullscreen)
    listboxwindow = winMgr:getWindow("orxonox/ResolutionListbox")
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
        item = CEGUI.createListboxTextItem(v)
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toListbox(listboxwindow):addItem(item)
    end
    if resolution == "640 x 480" then
        listboxwindow:setItemSelectState(0,true)
    elseif resolution == "720 x 480" then
        listboxwindow:setItemSelectState(1,true)
    elseif resolution == "720 x 576" then
        listboxwindow:setItemSelectState(2,true)
    elseif resolution == "800 x 480" then
        listboxwindow:setItemSelectState(3,true)
    elseif resolution == "800 x 600" then
        listboxwindow:setItemSelectState(4,true)
    elseif resolution == "1024 x 480" then
        listboxwindow:setItemSelectState(5,true)
    elseif resolution == "1024 x 600" then
        listboxwindow:setItemSelectState(6,true)
    elseif resolution == "1024 x 768" then
        listboxwindow:setItemSelectState(7,true)
    elseif resolution == "1152 x 864" then
        listboxwindow:setItemSelectState(8,true)
    elseif resolution == "1280 x 720" then
        listboxwindow:setItemSelectState(9,true)
    elseif resolution == "1280 x 768" then
        listboxwindow:setItemSelectState(10,true)
    elseif resolution == "1440 x 900" then
        listboxwindow:setItemSelectState(11,true)
    end
    scrollbar_active = false
    block = false
end

function P.GraphicsResolutionListbox_changed(e)
    if listboxwindow:isItemSelected(0) then
        resolution = "640 x 480"
    elseif listboxwindow:isItemSelected(1) then
        resolution = "720 x 480"
    elseif listboxwindow:isItemSelected(2) then
        resolution = "720 x 576"
    elseif listboxwindow:isItemSelected(3) then
        resolution = "800 x 480"
    elseif listboxwindow:isItemSelected(4) then
        resolution = "800 x 600"
    elseif listboxwindow:isItemSelected(5) then
        resolution = "1024 x 480"
    elseif listboxwindow:isItemSelected(6) then
        resolution = "1024 x 600"
    elseif listboxwindow:isItemSelected(7) then
        resolution = "1024 x 768"
    elseif listboxwindow:isItemSelected(8) then
        resolution = "1152 x 864"
    elseif listboxwindow:isItemSelected(9) then
        resolution = "1280 x 720"
    elseif listboxwindow:isItemSelected(10) then
        resolution = "1280 x 768"
    elseif listboxwindow:isItemSelected(11) then
        resolution = "1440 x 900"
    end
    search_mode = 0
    f = io.open(file, "r")
    firstline = f:read("*line")
    text = firstline .. "\n"
    rendersystem = string.sub(firstline, 15)
    for line in f:lines() do
        if search_mode == 0 then
            if string.find(line, rendersystem) ~= nil then
                search_mode = 1
            end
        end
        if search_mode == 1 then
            if string.sub(line, 1, 10) == "Video Mode" then
                if string.match(line, "@") == "@" then
                    line = "Video Mode=" .. resolution .. string.sub(line, string.find(line, "@")-1)
                else
                    line = "Video Mode=" .. resolution
                end
                search_mode = 2
            end
        end
        text = text .. line .. "\n"
    end
    f:close()
    f = io.open(file, "w")
    f:write(text)
    f:close()
end

function P.GraphicsBrightnessScrollbar_changed(e)
    if scrollbar_active == false then
        -- brightness
        debug("event: brightness")
    end
end

function P.GraphicsBrightnessScrollbar_started(e)
    scrollbar_active = true
end

function P.GraphicsBrightnessScrollbar_ended(e)
    -- brightness
    debug("event: brightness")
    scrollbar_active = false
end

function P.GraphicsFullscreenCheckbox_clicked(e)
    if block == false then
        search_mode = 0
        f = io.open(file, "r")
        firstline = f:read("*line")
        text = firstline .. "\n"
        rendersystem = string.sub(firstline, 15)
        for line in f:lines() do
            if search_mode == 0 then
                if string.find(line, rendersystem) ~= nil then
                    search_mode = 1
                end
            end
            if search_mode == 1 then
                if string.sub(line, 1, 11) == "Full Screen" then
                    if fullscreen == true then
                        line = "Full Screen=No"
                        fullscreen = false
                    else
                        line = "Full Screen=Yes"
                        fullscreen = true
                    end
                    search_mode = 2
                end
            end
            text = text .. line .. "\n"
        end
        f:close()
        f = io.open(file, "w")
        f:write(text)
        f:close()
    end
end

function P.GraphicsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

