-- Set some test variables
x0 = 0
y0 = 500
z0 = 500

xo = 2000 
yo = 0
zo = 0

x1 = 3000
y1 = 0 
z1 = 0

x2 = 2000
y2 = 2000 
z2 = 0

-- Get a local pointer to a scriptcontroller
local ctrl = orxonox.ScriptController:getScriptController()

-- If it worked, call its "movetoposition" function
if ctrl ~= nil then

  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("chl", x0, y0, z0, xo, yo, zo, 2)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("mal", x2, y2, z2, xo, yo, zo, 2)
  ctrl:eventScheduler("mal", x1, y1, z1, xo, yo, zo, 2)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("ral", xo, yo, zo, 3, 1000, 0, 5)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("ral", xo, yo, zo, 2, 1000, 0, 5)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("ral", xo, yo, zo, 1, 1000, 0, 5)
  ctrl:eventScheduler("idle", 1)

end