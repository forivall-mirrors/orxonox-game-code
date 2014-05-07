--k = 1.2

--orxonox.execute("orxout message test " .. k)

x = 1.1
y = 2.2
z = 3.3


local ctrl = orxonox.ScriptController:getScriptController()
if ctrl ~= nil then
  ctrl:moveToPosition_beta(x, y, z)
end

--ctrl.moveToPosition(x,y,z)
--
if newctrlid ~= nil then
  orxonox.execute("orxout message test " .. newctrlid)
end

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


