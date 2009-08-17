-- Note: luaState is a pointer to the LuaState instance that created this lua state

-- Redirect debug to print
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
end
dofile = doFile

-- Create includeFile function that preparses the file according
-- to a function provided to the LuaState constructor (in C++)
-- Note: See the same notes as for doFile
include = function(filename, resourceGroup)
  local bSearchOtherPaths = (resourceGroup == nil) or false
  resourceGroup = resourceGroup or "NoResourceGroupProvided"
  luaState:includeFile(filename, resourceGroup, bSearchOtherPaths)
end
