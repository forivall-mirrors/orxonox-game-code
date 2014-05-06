--k = 1.2

--orxonox.execute("orxout message test " .. k)

x = 1.1
y = 2.2
z = 3.3


local ctrl = orxonox.ScriptController:getScriptController()
--orxonox.ScriptController:moveToPosition(x, y, z)
--ctrl.moveToPosition(x,y,z)

local docks = orxonox.Dock:getNumberOfActiveDocks()
local docklist = {}
for i = 0, docks-1 do
  table.insert(docklist, orxonox.Dock:getActiveDockAtIndex(i))
end
local dock = docklist[1]
if dock ~= nil then
    dock:dock()
end

--orxonox.execute("setPause 1")


