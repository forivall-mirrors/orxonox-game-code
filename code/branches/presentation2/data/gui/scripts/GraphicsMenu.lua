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
    dropdown = winMgr:getWindow("orxonox/ResolutionCombobox")
    local resolutionList = {}
    table.insert(resolutionList, "800 x 600  (4:3)")
    table.insert(resolutionList, "1024 x 640  (16:10)")
    table.insert(resolutionList, "1024 x 768  (4:3)")
    table.insert(resolutionList, "1280 x 800  (16:10)")
    table.insert(resolutionList, "1280 x 960  (4:3)")
    table.insert(resolutionList, "1440 x 900  (16:10)")
    table.insert(resolutionList, "1600 x 1200  (4:3)")
    table.insert(resolutionList, "1680 x 1050  (16:10)")
    for k,v in pairs(resolutionList) do
        item = CEGUI.createListboxTextItem(v)        
        item:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        CEGUI.toCombobox(dropdown):addItem(item)
    end
    local scrollbar_active = false
end

function P.GraphicsResolutionCombobox_changed(e)
    -- resolution
    debug("event: resolution")
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
    -- fullscreen
    debug("event: fullscreen")
end

function P.GraphicsBackButton_clicked(e)
    hideGUI(P.filename)
end

return P

