-- Note: luaState is a pointer to the LuaState instance that created this lua state

-- Redirect print to the C++ print function
original_print = print
print = function(s)
  luaState:luaPrint(s)
end

-- Create function to log text like COUT, but always prints a line!
logMessage = function(level, message)
  luaState:luaLog(level, message)
end
cout = logMessage

-- Redirect dofile in order to load with the resource manager
original_dofile = dofile
dofile = function(filename)
  luaState:doFile(filename)
  -- Required because the C++ function cannot return whatever might be on the stack
  return LuaStateReturnValue -- C-injected global variable
end
doFile = dofile

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
original_require = require
require = function(moduleName)
  if not luaState:fileExists(moduleName .. ".lua") then
    logMessage(2, "Warning: Lua function require() could not find file '" .. moduleName .. ".lua' ")
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

-- Convenience function for config values
orxonox.getConfig = function(section, entry)
  return orxonox.SettingsConfigFile:getInstance():getConfig(section, entry)
end
orxonox.config = function(section, entry, value)
  return orxonox.SettingsConfigFile:getInstance():config(section, entry, value)
end
orxonox.tconfig = function(section, entry, value)
  return orxonox.SettingsConfigFile:getInstance():tconfig(section, entry, value)
end

-- Include command line debugger for lua 5.1
if _VERSION ~= "Lua 5.0" then
  require("Debugger")
end
