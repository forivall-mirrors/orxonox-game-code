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
-- Note: The function does not behave exactly like LuaState::doFile because the
--       default argument here for the group is not "General" but
--       "NoResourceGroupProvided". This resolves to the resource group used to
--       do the current file.
doFile = function(filename, resourceGroup)
  local bSearchOtherPaths = (resourceGroup == nil) or false
  resourceGroup = resourceGroup or "NoResourceGroupProvided"
  luaState:doFile(filename, resourceGroup, bSearchOtherPaths)
  -- Required because the C++ function cannot return whatever might be on the stack
  return LuaStateReturnValue
end
original_dofile = dofile
dofile = doFile

-- Create includeFile function that preparses the file according
-- to a function provided to the LuaState constructor (in C++)
-- Note: See the same notes as for doFile
include = function(filename, resourceGroup)
  local bSearchOtherPaths = (resourceGroup == nil) or false
  resourceGroup = resourceGroup or "NoResourceGroupProvided"
  luaState:includeFile(filename, resourceGroup, bSearchOtherPaths)
  -- Required because the C++ function cannot return whatever might be on the stack
  return LuaStateReturnValue
end

-- Replace require function with almost similar behaviour
-- The difference is that you need to provide a resource group
-- Default value there is the current one (if present) or else "General"
-- But the loaded modules are then stored with only with the name (where name has no .lua extension)
-- CAUTION: That also means that you need to take care of conflicting filenames among groups
-- Furthermore the moduleName parameters is appended with the .lua extension when looking for the file
old_require = require
require = function(moduleName, resourceGroup)
  local bSearchOtherPaths = (resourceGroup == nil) or false
  resourceGroup = resourceGroup or "NoResourceGroupProvided"
  if not luaState:fileExists(moduleName .. ".lua", resourceGroup, bSearchOtherPaths) then
    return nil
  end
  if not _LOADED then
    _LOADED = {}
  end
  if not _LOADED[moduleName] then
    -- save old value
    _REQUIREDNAME_OLD = _REQUIREDNAME
    _REQUIREDNAME = moduleName
    luaState:doFile(moduleName .. ".lua", resourceGroup, bSearchOtherPaths)
    _LOADED[moduleName] = LuaStateReturnValue or true
    -- restore old value
    _REQUIREDNAME = _REQUIREDNAME_OLD
  end
  return _LOADED[moduleName]
end
