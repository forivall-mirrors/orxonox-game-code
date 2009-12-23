-- Note: luaState is a pointer to the LuaState instance that created this lua state

-- Save original print function in debug
debug = print

-- Redirect print to the C++ print function
print = function(s)
  luaState:luaPrint(s)
end

-- Create function to log text like COUT, but always prints a line!
logMessage = function(level, message)
  luaState:luaLog(level, message)
end

-- Redirect dofile in order to load with the resource manager
doFile = function(filename)
  luaState:doFile(filename)
  -- Required because the C++ function cannot return whatever might be on the stack
  return LuaStateReturnValue -- C-injected global variable
end
original_dofile = dofile
dofile = doFile

-- Create includeFile function that preparses the file according
-- to a function provided to the LuaState constructor (in C++)
include = function(filename)
  luaState:includeFile(filename)
  -- Required because the C++ function cannot return whatever might be on the stack
  return LuaStateReturnValue -- C-injected global variable
end

-- Replace require function with almost similar behaviour
-- The loaded modules are then stored with their names (where name has no .lua extension)
-- Furthermore the ".lua" extension is appended to the moduleName parameter when looking for the file
old_require = require
require = function(moduleName)
  if not luaState:fileExists(moduleName .. ".lua") then
    return nil
  end
  if not _LOADED then
    _LOADED = {}
  end
  if not _LOADED[moduleName] then
    -- save old value
    _REQUIREDNAME_OLD = _REQUIREDNAME
    _REQUIREDNAME = moduleName
    luaState:doFile(moduleName .. ".lua")
    _LOADED[moduleName] = LuaStateReturnValue or true
    -- restore old value
    _REQUIREDNAME = _REQUIREDNAME_OLD
  end
  return _LOADED[moduleName]
end

-- Convenience function for console commands
orxonox.execute = function(command)
  orxonox.CommandExecutor:execute(command)
end
