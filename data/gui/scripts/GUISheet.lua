-- GUISheet.lua

local P = {}
_G[_REQUIREDNAME or "GUISheet"] = P
P.__index = P

-- Don't use directly --> use HUDSheet.new or MenuSheet.new
function P.new(_name)
    local newSheet = { name = _name }
    setmetatable(newSheet, P)
    return newSheet
end

-- Override this function if you need to do work on load
function P:onLoad()
end

-- show function for the GUI
function P:show()
    self.window:show()
    self.bVisible = true

    self:onShow()
end

-- Override this function if you need to do work on show
function P:onShow()
end

-- hide function for the GUI
function P:hide()
    self.window:hide()
    self.bVisible = false

    self:onHide()
end

-- Override this function if you need to do work on hide
function P:onHide()
end

function P:load()
    -- Load the layout that describes the sheet
    self.window = winMgr:loadWindowLayout(self.name .. ".layout")
    if self.window == nil then
        error("Could not load layout file for GUI sheet '"..self.name.."'")
    end
    -- Hide it at first
    self:hide()
    -- Allow sheets to do some work upon loading
    self:onLoad()
    return self
end

return P
