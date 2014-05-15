--k = 1.2

--orxonox.execute("orxout message test " .. k)

-- Set some test variables
x = 1.1
y = 2.2
z = 3.3

--os.execute("sleep " .. 2)

-- Get a local pointer to a scriptcontroller
local ctrl = orxonox.ScriptController:getScriptController()

-- If it worked, call its "movetoposition" function
if ctrl ~= nil then
  ctrl:eventScheduler("moveToPosition_beta", x, y, z, 0.01)
end



-- Output the newctrlid variable we set from the C++ code
if newctrlid ~= nil then
  orxonox.execute("orxout message test " .. newctrlid)
end

--orxonox.execute("setPause 1")


