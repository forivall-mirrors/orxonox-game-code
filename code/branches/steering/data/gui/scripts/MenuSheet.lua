-- MenuSheet.lua
-- Base class for all GUI sheets that represent a menu.
-- Inherits itself from GUISheet

local P = {}  -- Local alias, always use it in this file
MenuSheet = P -- Global name
setmetatable(P, require("GUISheet")) -- Inherit from GUISheet
P.__index = P -- Provide class character

-- Use this function to construct a new MenuSheet.
-- Parameters:
-- Except for _name, you can provide nil. Then the default value will be used.
-- For _tShowCusor and _tUseKeyboard you can specify TriBool.Dontcare if the value doesn't matter at all. Then the value of the underlaying sheet will be used.
function P.new(_name, _bHidePrevious, _tShowCursor, _tUseKeyboard, _bBlockJoyStick)
    local newSheet = GUISheet.new(_name)
    newSheet.bHidePrevious  = handleDefArg(_bHidePrevious,  true)
    newSheet.tShowCursor    = handleDefArg(_tShowCusor,     TriBool.True)
    newSheet.tUseKeyboard   = handleDefArg(_tUseKeyboard,   TriBool.True)
    newSheet.bBlockJoyStick = handleDefArg(_bBlockJoyStick, false)

    setmetatable(newSheet, P)
    return newSheet
end

function P:load()
    -- Create the input state
    self.inputState = guiMgr:createInputState("GUI_" .. self.name,
        self.tShowCursor, self.tUseKeyboard, self.bBlockJoyStick)

    -- load() of base 'class'
    GUISheet.load(self)

    return self
end

return P
