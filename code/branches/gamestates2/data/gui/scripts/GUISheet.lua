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
-- TODO: rename to onLoad
function P:init()
end

-- hide function for the GUI
function P:hide()
    self.window:hide()
    self.bVisible = false
end

-- show function for the GUI
function P:show()
    self.window:show()
    self.bVisible = true
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
    self:init()
    return self
end

return P
