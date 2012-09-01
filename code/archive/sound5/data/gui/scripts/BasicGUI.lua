-- gui.lua

local P = {}
if _REQUIREDNAME == nil then
    BasicGUI = P
else
    _G[_REQUIREDNAME] = P
end

P.overlay = nil

-- constructor of the GUI
function P:new (gui, fname)
    local newElement = { window = gui, filename = fname, visible = false } or {}
    setmetatable(newElement, self) -- connects new element with class
    self.__index = self
    return newElement
end

function P:init()
-- this function is empty and intended for inheriting GUIs to use
end

-- hide function for the GUI
function P:hide ()
    self.window:hide()
    self.visible = false
end

-- show function for the GUI
function P:show ()
    self.window:show()
    self.visible = true
end

function P:load ()
    self.window = winMgr:loadWindowLayout(self.layoutString)
    self:init()
    return self
end

return P
