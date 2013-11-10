-- HUDSheet.lua

local P = {} -- Local alias, always use it in this file
HUDSheet = P -- Global name
setmetatable(P, require("GUISheet")) -- Inherit from GUISheet
P.__index = P -- Provide class character

-- Use this function to construct a new HUDSheet
function P.new(name)
    local newSheet = GUISheet.new(name)
    setmetatable(newSheet, P)
    return newSheet
end

return P
