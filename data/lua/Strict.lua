--
-- strict.lua
-- checks uses of undeclared global variables
-- All global variables must be 'declared' through a regular assignment
-- (even assigning nil will do) in a main chunk before being used
-- anywhere or assigned to inside a function.
--

local mt = getmetatable(_G)
if mt == nil then
  mt = {}
  setmetatable(_G, mt)
end

__STRICT = true
mt.__declared = {}

mt.__newindex = function (t, n, v)
  if not mt.__declared[n] then
    if __STRICT then
      local d = debug.getinfo(2, "S")
      local w = d and d.what or "C"
      if w ~= "main" and w ~= "C" then
        error("Assigning to undeclared global variable '"..n.."'", 2)
      end
    end
    mt.__declared[n] = true
  end
  rawset(t, n, v)
end
  
mt.__index = function (t, n)
  if not mt.__declared[n] then
    local d = debug.getinfo(2, "S")
    local w = d and d.what or "C"
    if w ~= "C" then
      error("Global variable '"..n.."' was not declared", 2)
    else
      mt.__declared[n] = true
    end
  end
  return rawget(t, n)
end

function global(...)
  for _, v in ipairs{...} do
    mt.__declared[v] = true
  end
end
