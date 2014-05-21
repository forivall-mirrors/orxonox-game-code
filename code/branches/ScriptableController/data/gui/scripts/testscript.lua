-- Set some test variables
xm = 1000 
ym = 0
zm = 0

xl = -1000 
yl = 3000 
zl = -1000

d = 5
Tabs = 0

pi = 3.1415

-- Get a local pointer to a scriptcontroller
local ctrl = orxonox.ScriptController:getScriptController()

-- If it worked, call its "movetoposition" function
if ctrl ~= nil then
  -- Move to the starting point at (xl+3000,yl,zl) while looking 
  -- at xl,yl,zl over the time span of 3 seconds
  ctrl:eventScheduler("mal", xl+3000,yl,zl, xl,yl,zl, 3, 0)

  -- From there, perform a rotation around the harvester placed there
  -- in 100 steps
  dt = math.pi/100
  for t = 0,math.pi,dt do
    xt = math.cos(t)
    yt = math.sin(t)

    ctrl:eventScheduler("mal", xl+3000*xt, yl+3000*yt, zl, xl, yl, zl, 3*dt, 3*t+2.9)
  end

  -- Update absolute time
  Tabs = 3*math.pi + 2.9

  -- Move away again, still looking at the station
  ctrl:eventScheduler("mal", 0,0,1000, xl,yl,zl, 3, Tabs+0.4)


end



-- Output the newctrlid variable we set from the C++ code
if newctrlid ~= nil then
  orxonox.execute("orxout message test " .. newctrlid)
end

--orxonox.execute("setPause 1")


