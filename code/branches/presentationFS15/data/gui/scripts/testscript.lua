-- Set some test variables
xm = 1000 
ym = 0
zm = 0

xl = -1000 
yl = 3000 
zl = -1000

xr = 2000
yr = 500
zr = 2000

d = 5
Tabs = 0

pi = 3.1415

-- Get a local pointer to a scriptcontroller
local ctrl = orxonox.ScriptController:getScriptController()

-- If it worked, call its "movetoposition" function
if ctrl ~= nil then

  ctrl:eventScheduler("mal", xl,yl,zl, xl,yl,zl, 2)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("ral", xl, yl, zl, 3, 0, 0, math.pi)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("mal", 0,0,1000, xl,yl,zl, 3)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("chl", xl, yl, zl, xr, yr, zr, 2)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("mal", xr + 600,yr,zr, xr,yr,zr, 4)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("chl", xr, yr, zr, xl, yl, zl, 3)
  ctrl:eventScheduler("idle", 1)
  ctrl:eventScheduler("spi", xm,ym,zm, 0,0,0, 4)
  ctrl:eventScheduler("idle", 1)

end

-- Output the newctrlid variable we set from the C++ code
if newctrlid ~= nil then
  orxonox.execute("orxout message test " .. newctrlid)
end

--orxonox.execute("setPause 1")