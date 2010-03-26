-- BasicGUI.lua

local P = {}
_G[_REQUIREDNAME or "BasicGUI"] = P

-- useless, even wrong? P is the class, not the object..
P.overlay = nil

-- constructor of the GUI
function P:new(_filename, _gui, _visible)
    local newElement = {
        filename = _filename,
        gui = _gui,
        visible = _visible or false
    } or {}
    setmetatable(newElement, self) -- connects new element with class
    self.__index = self
    return newElement
end

-- Override this function if you need to
function P:init()
end

-- Override this function if you want to change one of the three input parameters:
-- showCursor = true, useKeyboard = true and blockJoyStick = false
-- But don't forget to stick to the naming convention ("GUI_" .. self.filename)
function P:createInputState()
    self.inputState = guiMgr:createInputState("GUI_" .. self.filename)
end

-- hide function for the GUI
function P:hide()
    self.window:hide()
    self.visible = false
end

-- show function for the GUI
function P:show()
    self.window:show()
    self.visible = true
end

function P:load()
    self.window = winMgr:loadWindowLayout(self.filename .. ".layout")
    self:createInputState()
    self:init()
    return self
end

return P
