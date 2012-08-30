----------------------------------------------------------------------------------------------------
-- This lua script creates a totally random generated space station for the orxonox computer game!--
-- (c) Wallah 2008, published under GPL licence!                                                  --
----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- IMPORTANT: If you need more parameters, do the following: copy the actual function (just the headline and the end statement) to the end of the file,           --
-- like I did with createSpaceStation() and let that function call the new function where you can modify the parameters. For all parameters which the old function--
-- doesn't have you just give the standard default values, which I have defined. This is to make sure, that anyone else who uses the old function can still use it--
-- the same way he/she always did. If you want a function with less parameters, just create a new one at the end of the file and call this function with some     --
-- default values. REMEMBER: Function overloading is not possible, be sure to call your function differently from others already existing ones.                   --
--------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- This function creates a randomly generated space station.
-- The first argument ranSeed, must be 0, or a positive Integer, if it is 0 your space station is always chosen randomly, if you give an integer,
--  your space station will be generated randomly, but once you have the space station it will always be the same.
-- The argument xLength defines how large the space station will be into the x-direction.
-- The argument xVar defines how much the space station will vary at the ends in x-direction, this is so that the station is no cube.
-- The argument yLength is the same as xLength, but for the y-direction.
-- The argument yVar is the same as xLength, but for the y-direction.
-- The argument zLength is the same as xLength, but for the z-direction.
-- The argument zVar is the same as xLength, but for the z-direction.
-- The argument sSScale scales the station proportionally in all directions.
function createSpaceStationPar(ranSeed, xLength, xVar, yLength, yVar, zLength, zVar, sSScale)



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- This prints xml code, which creates a MovableEntity, which I need to attach all the parts of the space station.
print("<MovableEntity scale=1 position=\"0,0,0\" >")
-- End create Movable Entity.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Create a randomseed, so that the math.random() function is actually random.
if ranSeed == 0 then
    math.randomseed(os.time())
else
    math.randomseed(ranSeed)
end
-- End create randomseed.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Here you can define some global variables, with which you can modify the space station.

-- Define the maximal size of the grid, this value is based on how large you define the area of bodyparts plus 20 gridpoints for attachparts.
sSSize=30
if xLength>=yLength and xLength>=zLength then
    sSSize=xLength+20
elseif yLength>=xLength and yLength>=zLength then
    sSSize=yLength+20
elseif zLength>=xLength and zLength>=yLength then
    sSSize=zLength+20
end
-- Define how many parts the space station has, this value has to be exact, so be sure to increment it if you're adding a new part.
sSParts=9
-- Define how many body parts the space station has, this value has to be exact. Body part means a part, which has connections at least in two directions.
sSBodyParts=3
-- Define how many frontParts you have.
frontParts=1
-- Define how many backParts you have.
backParts=1
-- Define how many side parts for the left side you have.
leftSideParts=1
-- Define how many side parts for the right side you have.
rightSideParts=1
-- Define how many top parts you have.
topParts=2
-- Define how many connection parts you have.
connParts=1
-- Define the maximal dimension of a single part, be sure this value is big enough, better it's too big, it's only a matter of efficiency.
-- It should be at least 1 bigger than the biggest part needs, because I guarantee that it works with odd numbers, to do so I use the math.floor function.
pDim=6
-- Define the griddimension, be sure this value matches the size of a single space station part plus the size of a connection part, which means your parts must be:
--  integer*(gridDim-connectionSize), then integer tells you how many griddimensions your part is.
gridDim=2.25
-- End define global parameters.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- This creates a 4-dimensional grid, which tells us if there is a part or not, and in which direction it has connections.
-- The parameters x,y,z are the axis of the space station, which iterate to sSSize, the maximal size of the space station.
-- The griddimension, this word I will use later, means that the distance of a point to the next point is gridDim in the game, so the absolute x-axis is x*gridDim*sSScale,
--  and so on for the other dimensions y and z.
-- grid[x][y][z][0] contains 0 if there is no part at the position (x,y,z), otherwise 1.
-- grid[x][y][z][1] contains 0 if there is no connection from (x,y,z) in x-direction, "+" if there is one in the positive x-direction,
--  "-" if there is one in the negative x-direction, "+-" if there are in both x-directions.
-- grid[x][y][z][2] contains 0 if there is no connection from (x,y,z) in y-direction, "+" if there is one in the positive y-direction,
--  "-" if there is one in the negative y-direction, "+-" if there are in both y-directions.
-- grid[x][y][z][3] contains 0 if there is no connection from (x,y,z) in z-direction, "+" if there is one in the positive z-direction,
--  "-" if there is one in the negative z-direction, "+-" if there are in both z-directions.
grid = {}
for x=-math.floor(sSSize/2),math.floor(sSSize/2) do
    grid[x] = {}
    for y=-math.floor(sSSize/2),math.floor(sSSize/2) do
        grid[x][y]= {}
        for z=-math.floor(sSSize/2),math.floor(sSSize/2) do
            grid[x][y][z]={}
            for i=0,3 do
                grid[x][y][z][i]=0
            end
        end
    end
end
-- End create 4-dim grid.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- This creates an array which stores all the parts, it's size is depending on the global values pDim and sSParts.
-- The first parameter i, tells us how many parts fit into the array, so it iterates from 1 to sSParts, each part has his own value i.
-- The second, third and fourth parameters are the relative coordinates of the part, you have to start at (0,0,0) and be sure you fill the array into the right direction.
--  A short example: your part is 2 griddimensions long and you place it in the game, that the relative coordinate point is at (0,0,0) and the part lies in the positive
--  z-axis, then you have to use the coordinate point (0,0,1).
-- The fifth parameter is an array with size 4, at index=0, you have to set 1 if your part covers the gridpoint at (x,y,z), otherwise 0.
--  At index=1,2,3 you define the possible connection directions (1 for x, 2 for y and 3 for z), be sure to use the notation from above (0, "+-", "+", "-").
bodyParts={}
for i=1,sSParts do
    bodyParts[i]={}
    for x=-math.floor(pDim/2),math.floor(pDim/2) do
        bodyParts[i][x]={}
        for y=-math.floor(pDim/2),math.floor(pDim/2) do
            bodyParts[i][x][y]={}
            for z=-math.floor(pDim/2),math.floor(pDim/2) do
                bodyParts[i][x][y][z]={}
                for k=0,3 do
                    bodyParts[i][x][y][z][k]=0
                end
            end
        end
    end
    -- This contains the name of the mesh file.
    bodyParts[i][0][0][0][4]=""
    -- This contains the first possible rotation of your part, pitch=... yaw=... roll=... .
    bodyParts[i][0][0][0][5]=""
    -- This contains the second possible rotation of your part, pitch=... yaw=... roll=... .
    bodyParts[i][0][0][0][6]=""
    -- This contains the third possible rotation of your part, pitch=... yaw=... roll=... .
    bodyParts[i][0][0][0][7]=""
    -- Contains the movement rotation, rotationaxis=... rotationrate=... .
    bodyParts[i][0][0][0][8]=""
    -- Contains the attachment, if your part has an attachment, e.g. <ParticleEmitter .../>.
    bodyParts[i][0][0][0][9]=""
    -- Contains how many of this part you want to attach to your space station.
    bodyParts[i][0][0][0][10]=1
end
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Here you can add a part to the space station, there are some examples here and how to describe your part is written above in the commentary.

-- The part must be inserted so, that the center of reference is at position (0,0,0).
-- At position bodyParts[i][0][0][0][4] you have to put the mesh name of your part.
-- At bodyParts[i][0][0][0][5] you can rotate your part, with pitch=angle, yaw=angle or roll=angle (x,y or z). Positive angle means in screw direction.
-- At bodyParts[i][0][0][0][6] you have another possibility to rotate your part in a different way, e.g. for left and right side parts.
-- At bodyParts[i][0][0][0][7] you have a third possibility to rotate your part, e.g. connection parts must be rotated into 3 different ways.
-- At bodyParts[i][0][0][0][8] you can rotate your part around his own axis, with rotationaxis="x,y,z" rotationrate=number.
-- At bodyParts[i][0][0][0][9] you can attach something to your model, e.g. <ParticleEmitter .../>.

-- Insert the CuboidBody, which is only one griddimension and can have connections in every direction.
bodyParts[1][0][0][0][4]="CuboidBody.mesh"

bodyParts[1][0][0][0][0]=1
bodyParts[1][0][0][0][1]="+-"
bodyParts[1][0][0][0][2]="+-"
bodyParts[1][0][0][0][3]="+-"
-- End insert CuboidBody.

-- Insert the DoubleCuboidBody, which is two griddimensions long, and one wide and high and can have connections in every direction except in the middle.
bodyParts[2][0][0][0][4]="DoubleCuboidBody.mesh"
bodyParts[2][0][0][0][5]="pitch=-90"

bodyParts[2][0][0][0][0]=1
bodyParts[2][0][0][0][1]="+-"
bodyParts[2][0][0][0][2]="+-"
bodyParts[2][0][0][0][3]="-"

bodyParts[2][0][0][1][0]=1
bodyParts[2][0][0][1][1]="+-"
bodyParts[2][0][0][1][2]="+-"
bodyParts[2][0][0][1][3]="+"
-- End insert DoubleCuboidBody.

-- Insert the CuboidConnectionBody, it is three griddimensions long and one wide and high and can have only connections at griddimension 1
--  (except the side in direction of griddimension 2) and griddimension 3 (except the side in direction of griddimension 2).
bodyParts[3][0][0][0][4]="CuboidConnBody.mesh"
bodyParts[3][0][0][0][5]="pitch=-90"

bodyParts[3][0][0][0][0]=1
bodyParts[3][0][0][0][1]="+-"
bodyParts[3][0][0][0][2]="+-"
bodyParts[3][0][0][0][3]="-"

bodyParts[3][0][0][1][0]=1

bodyParts[3][0][0][2][0]=1
bodyParts[3][0][0][2][1]="+-"
bodyParts[3][0][0][2][2]="+-"
bodyParts[3][0][0][2][3]="+"
-- End insert CuboidConnectionBody.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Insert the back parts.
-- If you're space station has no back parts, be sure to set backPartsIndex[0]=false.
backPartsIndex={}
backPartsIndex[0]=""

-- Insert the thruster, which is one griddimension long, two wide and high, it can only have a connection into the negative z-direction.
backPartsIndex[1]=4
bodyParts[backPartsIndex[1]][0][0][0][4]="Thruster.mesh"
bodyParts[backPartsIndex[1]][0][0][0][5]="pitch=-90"
bodyParts[backPartsIndex[1]][0][0][0][9]="<ParticleEmitter position=\"0,-0.5,0\" source=\"Orxonox/ThrusterFire\" />"
bodyParts[backPartsIndex[1]][0][0][0][10]=5

bodyParts[backPartsIndex[1]][0][0][0][0]=1
bodyParts[backPartsIndex[1]][0][0][0][3]="-"
bodyParts[backPartsIndex[1]][1][0][0][0]=1
bodyParts[backPartsIndex[1]][-1][0][0][0]=1
bodyParts[backPartsIndex[1]][0][1][0][0]=1
bodyParts[backPartsIndex[1]][0][-1][0][0]=1
-- End insert the thruster.

-- End insert the back parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Insert the front parts. If your space station has no front parts, be sure to set frontPartsIndex[0]=false.
frontPartsIndex={}
frontPartsIndex[0]=""

-- The SemiCircleCockpit is 3 x-griddimensions long, 3 y-griddimensions and 2 z-griddimensions, it can only have a connection in the positive z-direction.
frontPartsIndex[1]=5
bodyParts[frontPartsIndex[1]][0][0][0][4]="SemiCircleCockpit.mesh"
bodyParts[frontPartsIndex[1]][0][0][0][5]="pitch=-90 yaw=180"
bodyParts[frontPartsIndex[1]][0][0][0][9]="<Billboard scale=0.01 position=\"0,-0.1,2.2\" colour=\"1.0,1.0,1.0\" material=\"Examples/Flare\" /> <Billboard scale=0.01 position=\"0.5,-0.1,2.2\" colour=\"1.0,1.0,1.0\" material=\"Examples/Flare\" /> <Billboard scale=0.01 position=\"-0.5,-0.1,2.2\" colour=\"1.0,1.0,1.0\" material=\"Examples/Flare\" />"

bodyParts[frontPartsIndex[1]][0][0][0][0]=1
bodyParts[frontPartsIndex[1]][0][0][0][3]="+"

bodyParts[frontPartsIndex[1]][-1][0][0][0]=1
bodyParts[frontPartsIndex[1]][1][0][0][0]=1
bodyParts[frontPartsIndex[1]][0][-1][0][0]=1
bodyParts[frontPartsIndex[1]][0][1][0][0]=1
bodyParts[frontPartsIndex[1]][-1][-1][0][0]=1
bodyParts[frontPartsIndex[1]][1][-1][0][0]=1
bodyParts[frontPartsIndex[1]][-1][1][0][0]=1
bodyParts[frontPartsIndex[1]][1][1][0][0]=1
bodyParts[frontPartsIndex[1]][0][0][-1][0]=1
bodyParts[frontPartsIndex[1]][-1][0][-1][0]=1
bodyParts[frontPartsIndex[1]][1][0][-1][0]=1
bodyParts[frontPartsIndex[1]][0][-1][-1][0]=1
bodyParts[frontPartsIndex[1]][0][1][-1][0]=1
bodyParts[frontPartsIndex[1]][-1][-1][-1][0]=1
bodyParts[frontPartsIndex[1]][1][-1][-1][0]=1
bodyParts[frontPartsIndex[1]][-1][1][-1][0]=1
bodyParts[frontPartsIndex[1]][1][1][-1][0]=1
-- End insert SemiCircleCockpit.

-- End insert the front parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Insert the side parts.
-- If your space station has no left side parts, be sure to set leftsidePartsIndex[0]=false.
-- If your space station has no right side parts, be sure to set rightsidePartsIndex[0]=false.
leftSidePartsIndex={}
leftSidePartsIndex[0]=""
rightSidePartsIndex={}
rightSidePartsIndex[0]=""

-- Insert the solar panel, which i wanna use as left and right side part.
leftSidePartsIndex[1]=6
rightSidePartsIndex[1]=leftSidePartsIndex[1]
bodyParts[leftSidePartsIndex[1]][0][0][0][4]="SolarPanel.mesh"
bodyParts[leftSidePartsIndex[1]][0][0][0][5]="roll=90 pitch="..math.random(0,180)
bodyParts[rightSidePartsIndex[1]][0][0][0][6]="roll=-90 pitch="..math.random(0,180)
bodyParts[rightSidePartsIndex[1]][0][0][0][8]="rotationaxis=\"1,0,0\" rotationrate=2"
bodyParts[leftSidePartsIndex[1]][0][0][0][0]=1
bodyParts[leftSidePartsIndex[1]][0][0][1][0]=1
bodyParts[leftSidePartsIndex[1]][0][0][-1][0]=1
bodyParts[leftSidePartsIndex[1]][0][1][0][0]=1
bodyParts[leftSidePartsIndex[1]][0][1][1][0]=1
bodyParts[leftSidePartsIndex[1]][0][1][-1][0]=1
bodyParts[leftSidePartsIndex[1]][0][-1][0][0]=1
bodyParts[leftSidePartsIndex[1]][0][-1][1][0]=1
bodyParts[leftSidePartsIndex[1]][0][-1][-1][0]=1
-- End insert solar panel.

-- End insert side parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Insert the top parts.
-- If you have no top parts, be sure to set topPartsIndex[0]=false
topPartsIndex={}
topPartsIndex[0]=""

-- Insert the CuboidLandingZone.
topPartsIndex[1]=7
bodyParts[topPartsIndex[1]][0][0][0][4]="CuboidLandingZone.mesh"
bodyParts[topPartsIndex[1]][0][0][0][5]="pitch=-90"

bodyParts[topPartsIndex[1]][0][0][0][0]=1
bodyParts[topPartsIndex[1]][0][0][0][2]="+-"
bodyParts[topPartsIndex[1]][1][0][0][0]=1
bodyParts[topPartsIndex[1]][-1][0][0][0]=1
bodyParts[topPartsIndex[1]][0][0][1][0]=1
bodyParts[topPartsIndex[1]][1][0][1][0]=1
bodyParts[topPartsIndex[1]][-1][0][1][0]=1
bodyParts[topPartsIndex[1]][0][0][2][0]=1
bodyParts[topPartsIndex[1]][1][0][2][0]=1
bodyParts[topPartsIndex[1]][-1][0][2][0]=1
bodyParts[topPartsIndex[1]][0][0][3][0]=1
bodyParts[topPartsIndex[1]][1][0][3][0]=1
bodyParts[topPartsIndex[1]][-1][0][3][0]=1
-- End insert the CuboidLandingZone.

-- Insert the SatelliteDish.
topPartsIndex[2]=8
bodyParts[topPartsIndex[2]][0][0][0][4]="satellitedish.mesh"
bodyParts[topPartsIndex[2]][0][0][0][5]="pitch=-90"
bodyParts[topPartsIndex[2]][0][0][0][8]="rotationaxis=\"0,1,0\" rotationrate=5"

bodyParts[topPartsIndex[2]][0][0][0][0]=1
bodyParts[topPartsIndex[2]][0][0][1][0]=1
bodyParts[topPartsIndex[2]][0][0][-1][0]=1
bodyParts[topPartsIndex[2]][1][0][0][0]=1
bodyParts[topPartsIndex[2]][1][0][1][0]=1
bodyParts[topPartsIndex[2]][1][0][-1][0]=1
bodyParts[topPartsIndex[2]][-1][0][0][0]=1
bodyParts[topPartsIndex[2]][-1][0][1][0]=1
bodyParts[topPartsIndex[2]][-1][0][-1][0]=1
-- End insert the SatelliteDish.

-- End insert the top parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Insert the connection parts, which are used to connect all the bodyparts.
-- If you're spacestation has no connection parts, be sure to set connPartsIndex[0]=false.
connPartsIndex={}
connPartsIndex[0]=""

-- Insert the CuboidConnection.
connPartsIndex[1]=9
bodyParts[connPartsIndex[1]][0][0][0][4]="CuboidConnection.mesh"
bodyParts[connPartsIndex[1]][0][0][0][5]="roll=90"
bodyParts[connPartsIndex[1]][0][0][0][6]=""
bodyParts[connPartsIndex[1]][0][0][0][7]="pitch=90"
-- End insert the CuboidConnection.

-- End insert the connection parts.

-- End create array bodyParts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Here I define some functions which I will use later.

--This function actualizes the grid, which I have to call always after I have added a new part to the space station.
function actualizeGrid(Index,x,y,z)
    for i=math.floor(-pDim/2)+1,math.floor(pDim/2) do
        for j=math.floor(-pDim/2)+1,math.floor(pDim/2) do
            for k=math.floor(-pDim/2)+1,math.floor(pDim/2) do
                if bodyParts[Index][i][j][k][0] == 1 then
                    for l=0,3 do
                        grid[x+i][y+j][z+k][l] = bodyParts[Index][i][j][k][l]
                    end
                end
            end
        end
    end
end
-- End actualizeGrid.

-- This function checks wheter a given parts fits at that position or not.
-- If the part fits there it returns 1, otherwise 0.
function checkPart(Index,x,y,z)
    check=1
    for i=math.floor(-pDim/2)+1,math.floor(pDim/2) do
        for j=math.floor(-pDim/2)+1,math.floor(pDim/2) do
            for k=math.floor(-pDim/2)+1,math.floor(pDim/2) do
                -- If the part occupies the position (i,j,k), the grid must be empty there ((x+i, y+j, z+k)==0), if not, check is zero,
                --  which means that the part doesn't fit there.
                if bodyParts[Index][i][j][k][0] == 1 and grid[x+i][y+j][z+k][0] == 1 then
                    check=0
                end
            end
        end
    end
    return check
end
-- End checkPart function.

-- This function prints the model with tempPartIndex in the bodyParts array at position lx,ly,lz.
-- If you need to rotate the model around his own axis, then you have to set movEntity true and define the details of the rotation in
--  bodyParts[tempPartIndex][0][0][0][8].
-- If your model needs to be rotated like bodyParts[tempPartIndex][0][0][0][5], then side must be 1, for bodyParts[tempPartIndex][0][0][0][6] side must be 2,
--  for bodyParts[tempPartIndex][0][0][0][7] side must be 3.
function printModel(lx,ly,lz,tempPartIndex,movEntity,side)
    if movEntity == true then
        print("<MovableEntity scale=1 position=\"") print(lx*gridDim*sSScale) print(",") print(ly*gridDim*sSScale) print(",") print(lz*gridDim*sSScale) print("\" ")
        print(bodyParts[tempPartIndex][0][0][0][8]) print(">")
        print("<attached>")
        lx=0 ly=0 lz=0
    end

    print("<Model position=\"") print(lx*gridDim*sSScale) print(",") print(ly*gridDim*sSScale) print(",") print(lz*gridDim*sSScale)
    print("\" scale=") print(sSScale) print(" mesh= \"") print(bodyParts[tempPartIndex][0][0][0][4]) print("\"")

        if side == 1 then
            print(bodyParts[tempPartIndex][0][0][0][5]) print(">")
        elseif side == 2 then
            print(bodyParts[tempPartIndex][0][0][0][6]) print(">")
        elseif side == 3 then
            print(bodyParts[tempPartIndex][0][0][0][7]) print(">")
        end

        print("<attached>")
            print(bodyParts[tempPartIndex][0][0][0][9])
        print("</attached>")

    print("</Model>")

    if movEntity == true then
        print("</attached>")
        print("</MovableEntity>")
    end
end
-- End function printModel().

-- This function sets a part to a side of the space station.
-- The arguments lx,ly,lz are the coordinates of the grid, where you want to set the part.
-- The arguments xAxis,yAxis,zAxis can be 0 or 1, but only one argument out of the three can be 1. This means two of them must always be zero. You have to set xAxis to one,
--  if your part is attached to a side, which faces into the x-direction (negative or positive, this is later specified with Dir), that means the x-Axis is a normal vector
--  of the side to which you want to attach the part. The same for yAxis and zAxis.
-- The argument Dir must be 1 if your side, where you want to attach the part, faces into the positive direction, -1 if the side faces into the negative direction. The side
--  faces into the positive direction means, that the side of the side where the part will be attached is directed into the direction of the positive direction of the
--  corresponding axis.
-- The argument index is the index of the part for the bodyParts array.
-- The argument printMovEnt must be false if your part doesn't need to be attached to an extra MovableEntity. If your part must be attached to an extra MovableEntity
--  this argument must be true. The extra MovableEntity is used to rotate the part around his own axis, or something like that.
-- The argument printSide is like the argument side of the printModel() function. It defines how your part will be rotated. Read the commentary there.
-- The function returns 0 if the part couldn't be set, because it did not fit there or there was no side to attach the part. It returns 1 if the part is successfully set.
function setPart(lx,ly,lz,xAxis,yAxis,zAxis,Dir,index,printMovEnt,printSide)

    partSet=0
    -- For the bodyParts array I use 1 as x-, 2 as y- and 3 as z-Axis for the definition in which directions a part can have connections.
    coord=1*xAxis+2*yAxis+3*zAxis
    -- If I try to attach the part from the positive direction to the side of the space station, the part of the station (where I want to attach the new part) must have
    --  a connection into the positive direction. Otherwise I look from the negative side and so the part of the station must have a connection into the negative direction.
    if Dir==1 then
        conn="+"
    elseif Dir==-1 then
        conn="-"
    end
    -- I look from the direction defined through Dir, and here I check, whether I have reached a side of the space station, which means at position lx,ly,lz is nothing and
    --  at the next position is a part which can have a connection into the direction from where I look.
    if grid[lx][ly][lz][0] == 0 and grid[lx+(-1*xAxis*Dir)][ly+(-1*yAxis*Dir)][lz+(-1*zAxis*Dir)][0] == 1 and (grid[lx+(-1*xAxis*Dir)][ly+(-1*yAxis*Dir)][lz+(-1*zAxis*Dir)][coord]=="+-" or grid[lx+(-1*xAxis*Dir)][ly+(-1*yAxis*Dir)][lz+(-1*zAxis*Dir)][coord]==conn) then
        -- This checks whether the part fits at that position or not.
        check=checkPart(index,lx,ly,lz)
        if check == 1 then
            -- This prints the part.
            printModel(lx,ly,lz,index,printMovEnt,printSide)
            partSet=1
            -- This actualizes the grid array with the values of the array bodyParts at the position index.
            actualizeGrid(index,lx,ly,lz)
        end
    end
    return partSet
end
-- End function setPart().

-- This function sets a part to a side of the space station. It is called spiralSet, because it starts in the middle of the side and goes out in a form of a spiral.
-- The argument xAxis,yAxis,zAxis,Dir,printMovEnt,printSide are the same as the arguments from the setPart() function, please read the commentary there.
-- The argument index here must be an array, where you define the index for your part for the bodyParts array. The first used index is 1 and goes up to parts.
-- The argument parts is the number of different parts which you want to attach to a side.
function spiralSet(xAxis,yAxis,zAxis,Dir,index,parts,printMovEnt,printSide)
    if index[0] ~= false then
        -- The array vector contains the actual position where you try to set the part. vector[0],vector[1] and vector[3] contains the x,y,z-coordinate.
        vector={}
        -- This must be done, because there are different sides from where I try to attach a part.
        coord1=1*yAxis+2*zAxis
        coord2=math.mod(coord1+1,3)
        coord3=math.mod(coord2+1,3)

        for pc=1,parts do
            tempIndex = index[pc]
            for eachPart=1,bodyParts[tempIndex][0][0][0][10] do
                partSet=0
                vector[coord1]=math.floor(Dir*sSSize/2)-2*Dir
                while vector[coord1]~=math.floor(-1*Dir*sSSize/2)+2*Dir and partSet==0 do
                    round=0
                    while round<=math.floor(sSSize/2)-2 and partSet==0 do
                        vector[coord2]=round
                        vector[coord3]=-round
                        while vector[coord3]<=round and partSet==0 do
                            partSet=setPart(vector[0],vector[1],vector[2],xAxis,yAxis,zAxis,Dir,tempIndex,printMovEnt,printSide)
                            vector[coord3]=vector[coord3]+1
                        end
                        while vector[coord2]>=-round and partSet==0 do
                            partSet=setPart(vector[0],vector[1],vector[2],xAxis,yAxis,zAxis,Dir,tempIndex,printMovEnt,printSide)
                            vector[coord2]=vector[coord2]-1
                        end
                        while vector[coord3]>-round and partSet==0 do
                            partSet=setPart(vector[0],vector[1],vector[2],xAxis,yAxis,zAxis,Dir,tempIndex,printMovEnt,printSide)
                            vector[coord3]=vector[coord3]-1
                        end
                        while vector[coord2]<=round and partSet==0 do
                            partSet=setPart(vector[0],vector[1],vector[2],xAxis,yAxis,zAxis,Dir,tempIndex,printMovEnt,printSide)
                            vector[coord2]=vector[coord2]+1
                        end
                        round=round+1
                    end
                    vector[coord1]=vector[coord1]-Dir
                end
            end
        end
    end
end
-- End function spiralSet().

-- End define functions.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- This is xml code, which means now we attach some parts to the MovableEntity.
print("<attached>")
-- End attach to the MovableEntity.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach all bodyparts.
-- Define at which position in the x-direction you're space station will start.
x=math.random(-math.floor(xLength/2),-math.floor(xLength/2)+xVar)
-- Define at which position in the x-direction you're space station will end.
xMax=math.random(math.floor(xLength/2),math.floor(xLength/2)+xVar)
while x<xMax do
    -- The same for the y- and z-direction.
    y=math.random(-math.floor(yLength/2),-math.floor(yLength/2)+yVar)
    yMax=math.random(math.floor(yLength/2),math.floor(yLength/2)+yVar)
    while y<yMax do
        yMax=math.random(math.floor(yLength/2),math.floor(yLength/2)+yVar)
        z=math.random(-math.floor(zLength/2),-math.floor(zLength/2)+zVar)
        zMax=math.random(math.floor(zLength/2),math.floor(zLength/2)+zVar)
        while z<zMax do
            -- This loop choses a bodypart, which fits at position (x,y,z).
            -- If after the fifth time the part does still not fit we terminate the loop and set no part at postition (x,y,z).
            partSet=0
            counter=0
            while counter<5 and partSet==0 do
                -- This choses randomly a bodyPartIndex, which is the index used for the parts in the array bodyParts.
                tempBodyPartIndex=math.random(1,sSBodyParts)
                check=checkPart(tempBodyPartIndex,x,y,z)
                -- If check == 1, this means that the part fits there, so we put it there and break the while true loop, to go on.
                if check == 1 then
                    -- This prints the chosen part at position (x*gridDim*sSScale,y*gridDim*sSScale,z*gridDim*sSScale).
                    printModel(x,y,z,tempBodyPartIndex,false,1)
                    -- This actualizes the grid array with the values of the array bodyParts at the position tempBodyPartIndex, which is our randomly chosen part.
                    actualizeGrid(tempBodyPartIndex,x,y,z)
                    partSet=1
                end
                counter=counter+1
            end
            z=z+1
        end
        y=y+1
    end
    x=x+1
end
-- End attach all bodyparts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach backParts, if there are some.
    spiralSet(0,0,1,1,backPartsIndex,backParts,false,1)
-- End attach backParts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach frontParts, if there are.
    spiralSet(0,0,1,-1,frontPartsIndex,frontParts,false,1)
-- End attach frontParts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach parts on the left side of the space station.
    spiralSet(1,0,0,-1,leftSidePartsIndex,leftSideParts,true,1)
-- End attach left side parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach parts on the right side of the space station.
    spiralSet(1,0,0,1,rightSidePartsIndex,rightSideParts,true,2)
-- End attach right side parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach parts on top of the space station.
    spiralSet(0,1,0,1,topPartsIndex,topParts,true,1)
-- End attach top parts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Attach all connectionparts.
-- This iterates through the whole grid array.
if connPartsIndex[0] ~= false then
    for x=math.floor(-sSSize/2)+2,math.floor(sSSize/2)-2 do
        for y=math.floor(-sSSize/2)+2,math.floor(sSSize/2)-2 do
            for z=math.floor(-sSSize/2)+2,math.floor(sSSize/2)-2 do
                tempConnPartIndex=connPartsIndex[math.random(1,connParts)]
                -- This checks whether there has to be a connection part between (x,y,z) and (x+1,y,z) or not. First it checks if there is a part at (x,y,z) and
                --  then it checks if that part can have a connection into the positive x-direction, if it can, it checks if there is a part at (x+1,y,z) and
                --  if that part can have a connection into the negative x-direction, if both can, it prints the xml code to set a connection part.
                if grid[x][y][z][0]==1 and (grid[x][y][z][1]=="+" or grid[x][y][z][1]=="+-") and grid[x+1][y][z][0]==1 and (grid[x+1][y][z][1]=="-" or grid[x+1][y][z][1]=="+-") then
                    -- This prints the connection part, the +1/2 is because the connection is set exactly in the middle of two gridpoints.
                    printModel(x+1/2,y,z,tempConnPartIndex,false,1)
                end
                -- The same as in the x-direction, but for the y-direction.
                if grid[x][y][z][0]==1 and ( grid[x][y][z][2]=="+" or grid[x][y][z][2]=="+-" ) and grid[x][y+1][z][0]==1 and ( grid[x][y+1][z][2]=="-" or grid[x][y+1][z][2]=="+-" ) then
                    printModel(x,y+1/2,z,tempConnPartIndex,false,2)
                end
                -- The same as in the x-direction, but for the z-direction.
                if grid[x][y][z][0]==1 and ( grid[x][y][z][3]=="+" or grid[x][y][z][3]=="+-" ) and grid[x][y][z+1][0]==1 and ( grid[x][y][z+1][3]=="-" or grid[x][y][z+1][3]=="+-" ) then
                    printModel(x,y,z+1/2,tempConnPartIndex,false,3)
                end
            end
        end
    end
end
-- End attach all connectionparts.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- This is xml code, which ends the attachment and the MovableEntity.
print("</attached>")
print("</MovableEntity>")
-- End ends attachment and MovableEntity.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------



end
-- End createSpaceStationPar() function.



-- This function is for the lazy guys, which do not care how the space station looks like, so I use some good standard values.
function createSpaceStation()
    createSpaceStationPar(0,4,1,2,1,6,1,100)
end
-- End createSpaceStaion() function.



