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
  if not luaState:doFile(filename) then
    error("Error propagation. Do not display")
  end
  -- Required because if the file returns a table, it cannot be passed through the C++ function
  return LuaStateReturnValue -- C-injected global variable
end
doFile = dofile

-- Create includeFile function that preparses the file according
-- to a function provided to the LuaState constructor (in C++)
include = function(filename)
  if not luaState:includeFile(filename) then
    error("Error propagation. Do not display")
  end
  -- Required because if the file returns a table, it cannot be passed through the C++ function
  return LuaStateReturnValue -- C-injected global variable
end

-- Replace require function with almost similar behaviour
-- The loaded modules are then stored with their names (where name has no .lua extension)
-- Furthermore the ".lua" extension is appended to the moduleName parameter when looking for the file
original_require = require
_REQUIREDNAME = ""
LuaStateReturnValue = true
require = function(moduleName)
  if not luaState:fileExists(moduleName .. ".lua") then
    logMessage(2, "Warning: Lua function require() could not find file '" .. moduleName .. ".lua' ")
    return nil
  end

  if not _LOADED then
    _LOADED = {}
  end
  if not _LOADED_RETURN_VALUES then
      _LOADED_RETURN_VALUES = {}
  end

  if not _LOADED[moduleName] then
    -- save old value for the required name
    local _REQUIREDNAME_OLD = _REQUIREDNAME
    _REQUIREDNAME = moduleName

    if not luaState:doFile(moduleName .. ".lua") then
      error("Error propagation. Do not display")
    end
    -- LuaStateReturnValue is required because if the file returns a table,
    -- it cannot be passed through the C++ function
    _LOADED_RETURN_VALUES[moduleName] = LuaStateReturnValue
    _LOADED[moduleName] = true

    -- restore old value
    _REQUIREDNAME = _REQUIREDNAME_OLD
  end
  local asdf = _LOADED_RETURN_VALUES[moduleName]
  return asdf
end


-- Load useful tool functions (like handleDefaultArgument)
require("Tools")


-- Include command line debugger for lua 5.1
-- Note: It doesn't work if the IOConsole was started. Then we replace pause() with a warning
if _VERSION ~= "Lua 5.0"  and not luaState:usingIOConsole() then
  require("Debugger")
else
  -- Fallback pause function
  pause = function()
    logMessage(2, [["Warning: debug() called in Lua, but Debugger is not active.
Do you have the IOConsole disabled and are you using Lua version 5.1?"]])
  end
end

-- General error handler that gets called whenever an error happens at runtime
errorHandler = function(err)
  if type(err) == "string" then
    -- Simply return if the error has already been handled
    if string.find(err, "Error propagation. Do not display") ~= nil then
      return err
    end
    -- Display the error message
    logMessage(1, "Lua runtime error: "..err)
  end

  -- Start debugger if possible
  if _LOADED and _LOADED["Debugger"] ~= nil then
    pause()
  else
    -- Fallback: print stack trace
    logMessage(3, debug.traceback(""))
  end
  return err -- Hello Lua debugger user! Please type 'set 2' to get to the
             -- actual position in the stack where the error occurred
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
