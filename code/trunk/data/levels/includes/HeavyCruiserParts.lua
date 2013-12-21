--[[ place all parts
places all sub-parts of HeavyCruiser as Pawns
  posX, posY, posZ - position in space
  dirX, dirY, dirZ - direction
  team - team
--]]

function placeAllSubParts(team, posX, posY, posZ, dirX, dirY, dirZ)
    placeSidearmL(team, posX, posY, posZ, dirX, dirY, dirZ)
    placeSidearmLfront(team, posX, posY, posZ, dirX, dirY, dirZ)
    placeSidearmR(team, posX, posY, posZ, dirX, dirY, dirZ)
    placeSidearmRfront(team, posX, posY, posZ, dirX, dirY, dirZ)
    placeFrontL(team, posX, posY, posZ, dirX, dirY, dirZ)
    placeFrontR(team, posX, posY, posZ, dirX, dirY, dirZ)
    placePartL(team, posX, posY, posZ, dirX, dirY, dirZ)
    placePartR(team, posX, posY, posZ, dirX, dirY, dirZ)
end


--[[ place individual parts
places sub-parts of HeavyCruiser as Pawns
  posX, posY, posZ - position in space
  dirX, dirY, dirZ - direction arguments
  team - team
--]]

function placeSidearmL(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=300 initialhealth=300 maxhealth=300 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_sidearmL radarname = \"HIDDEN\" >")
    print("<attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_sidearmL.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"-11,18,102\" halfExtents=\"10,10,10\" info=\"HOOK, sidearmL\"/> <BoxCollisionShape position=\"-151,47,115\" roll=\"15\" halfExtents=\"16,2,105\" info=\"sidearmL, frontTop\"/> <BoxCollisionShape position=\"-136,-7,115\" roll=\"15\" halfExtents=\"16,2,105\" info=\"sidearmL, frontBottom\"/> <BoxCollisionShape position=\"-127,26,115\" roll=\"15\" halfExtents=\"2,28,105\" info=\"sidearmL, frontInner\"/> <BoxCollisionShape position=\"-161,16,115\" roll=\"15\" halfExtents=\"2,28,105\" info=\"sidearmL, frontOuter\"/> <BoxCollisionShape position=\"-145,21,12\" roll=\"15\" halfExtents=\"16,28,2\" info=\"sidearmL, front\"/> <BoxCollisionShape position=\"-140,12,261\" roll=\"15\" halfExtents=\"22,41,43\" info=\"sidearmL, back\"/> <BoxCollisionShape position=\"-133,-13,29\" roll=\"15\" halfExtents=\"12,6,17\" info=\"sidearmL, bottom cylinder\"/> <BoxCollisionShape position=\"-165,-4,31\" roll=\"15\" halfExtents=\"7,21,13\" info=\"sidearmL, box\"/> <BoxCollisionShape position=\"-153,59,96\" roll=\"15\" halfExtents=\"6,7,60\" info=\"sidearmL, upper beam\"/> <BoxCollisionShape position=\"-143,57,119\" roll=\"15\" halfExtents=\"3,4,85\" info=\"sidearmL, upper inner beam\"/> <BoxCollisionShape position=\"-142,53,212\" pitch=\"21\" yaw=\"5\" roll=\"15\" halfExtents=\"3,4,10\" info=\"sidearmL, upper inner beam\"/> <BoxCollisionShape position=\"-160,52,119\" roll=\"15\" halfExtents=\"3,4,85\" info=\"sidearmL, upper outer beam\"/> <BoxCollisionShape position=\"-159,48,212\" pitch=\"21\" yaw=\"5\" roll=\"15\" halfExtents=\"3,4,10\" info=\"sidearmL, upper outer beam\"/> <BoxCollisionShape position=\"-152,54,34\" pitch=\"-46\" yaw=\"-14\" roll=\"15\" halfExtents=\"6,7,10\" info=\"sidearmL, upper beam front\"/> <BoxCollisionShape position=\"-152,54,162\" pitch=\"30\" yaw=\"9\" roll=\"15\" halfExtents=\"6,7,12\" info=\"sidearmL, upper beam back\"/> <BoxCollisionShape position=\"-168,21,119\" roll=\"15\" halfExtents=\"3,5,109\" info=\"sidearmL, outer beam\"/> <BoxCollisionShape position=\"-134,-10,169\" roll=\"15\" halfExtents=\"6,4,55\" info=\"sidearmL, lower beam front\"/> <BoxCollisionShape position=\"-131,-20,217\" pitch=\"-20\" roll=\"15\" yaw=\"-5\" halfExtents=\"6,12,3\" info=\"sidearmL, lower beam middle\"/> <BoxCollisionShape position=\"-130,-29,233\" roll=\"15\" halfExtents=\"6,3,15\" info=\"sidearmL, lower beam back\"/> <BoxCollisionShape position=\"-145,21,71\" halfExtents=\"10,10,1\" info=\"sidearmL, HOOKBOX\"/> <BoxCollisionShape position=\"-145,21,29\" halfExtents=\"10,10,1\" info=\"sidearmL, HOOKBOX\"/> <BoxCollisionShape position=\"-145,32,50\" halfExtents=\"10,1,20\" info=\"sidearmL, HOOKBOX\"/> <BoxCollisionShape position=\"-145,10,50\" halfExtents=\"10,1,20\" info=\"sidearmL, HOOKBOX\"/> <BoxCollisionShape position=\"-156,21,50\" halfExtents=\"1,10,20\" info=\"sidearmL, HOOKBOX\"/> <BoxCollisionShape position=\"-134,21,50\" halfExtents=\"1,10,20\" info=\"sidearmL, HOOKBOX\"/> </collisionShapes>")
    print("</Pawn>")
end


function placeSidearmLfront(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=100 initialhealth=100 maxhealth=100 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_sidearmLfront radarname = \"HIDDEN\" >")
    print(" <attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_sidearmLfront.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"-145,21,50\" halfExtents=\"10,10,20\" info=\"sidearmLfront, HOOK\"/> <BoxCollisionShape position=\"-141,15,-53\" roll=\"15\" halfExtents=\"27,44,62\" info=\"sidearmLfront\"/> <BoxCollisionShape position=\"-153,59,-30\" roll=\"15\" halfExtents=\"4,2,38\" info=\"sidearmLfront, top\"/> <BoxCollisionShape position=\"-153,61,-41\" roll=\"15\" halfExtents=\"4,4,11\" info=\"sidearmLfront, top\"/> <BoxCollisionShape position=\"-153,60,-65\" roll=\"15\" halfExtents=\"4,3,4\" info=\"sidearmLfront, top\"/> <BoxCollisionShape position=\"-153,59,-41\" roll=\"15\" halfExtents=\"8,2,11\" info=\"sidearmLfront, top\"/> </collisionShapes>")
    print("</Pawn>")
end


function placeSidearmR(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=300 initialhealth=300 maxhealth=300 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_sidearmR radarname = \"HIDDEN\" >")
    print("<attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_sidearmR.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"11,18,102\" halfExtents=\"10,10,10\" info=\"HOOK, sidearmR\"/> <BoxCollisionShape position=\"151,47,115\" roll=\"-15\" halfExtents=\"16,2,105\" info=\"sidearmR, frontTop\"/> <BoxCollisionShape position=\"136,-7,115\" roll=\"-15\" halfExtents=\"16,2,105\" info=\"sidearmR, frontBottom\"/> <BoxCollisionShape position=\"127,26,115\" roll=\"-15\" halfExtents=\"2,28,105\" info=\"sidearmR, frontInner\"/> <BoxCollisionShape position=\"161,16,115\" roll=\"-15\" halfExtents=\"2,28,105\" info=\"sidearmR, frontOuter\"/> <BoxCollisionShape position=\"145,21,12\" roll=\"-15\" halfExtents=\"16,28,2\" info=\"sidearmR, front\"/> <BoxCollisionShape position=\"140,12,261\" roll=\"-15\" halfExtents=\"22,41,43\" info=\"sidearmR, back\"/> <BoxCollisionShape position=\"133,-13,29\" roll=\"-15\" halfExtents=\"12,6,17\" info=\"sidearmR, bottom cylinder\"/> <BoxCollisionShape position=\"165,-4,31\" roll=\"-15\" halfExtents=\"7,21,13\" info=\"sidearmR, box\"/> <BoxCollisionShape position=\"153,59,96\" roll=\"-15\" halfExtents=\"6,7,60\" info=\"sidearmR, upper beam\"/> <BoxCollisionShape position=\"143,57,119\" roll=\"-15\" halfExtents=\"3,4,85\" info=\"sidearmR, upper inner beam\"/> <BoxCollisionShape position=\"142,53,212\" pitch=\"21\" yaw=\"-5\" roll=\"-15\" halfExtents=\"3,4,10\" info=\"sidearmR, upper inner beam\"/> <BoxCollisionShape position=\"160,52,119\" roll=\"-15\" halfExtents=\"3,4,85\" info=\"sidearmR, upper outer beam\"/> <BoxCollisionShape position=\"159,48,212\" pitch=\"21\" yaw=\"-5\" roll=\"-15\" halfExtents=\"3,4,10\" info=\"sidearmR, upper outer beam\"/> <BoxCollisionShape position=\"152,54,34\" pitch=\"-46\" yaw=\"14\" roll=\"-15\" halfExtents=\"6,7,10\" info=\"sidearmR, upper beam front\"/> <BoxCollisionShape position=\"152,54,162\" pitch=\"30\" yaw=\"-9\" roll=\"-15\" halfExtents=\"6,7,12\" info=\"sidearmR, upper beam back\"/> <BoxCollisionShape position=\"168,21,119\" roll=\"-15\" halfExtents=\"3,5,109\" info=\"sidearmR, outer beam\"/> <BoxCollisionShape position=\"134,-10,169\" roll=\"-15\" halfExtents=\"6,4,55\" info=\"sidearmR, lower beam front\"/> <BoxCollisionShape position=\"131,-20,217\" pitch=\"-20\" roll=\"-15\" yaw=\"5\" halfExtents=\"6,12,3\" info=\"sidearmR, lower beam middle\"/> <BoxCollisionShape position=\"130,-29,233\" roll=\"-15\" halfExtents=\"6,3,15\" info=\"sidearmR, lower beam back\"/> <BoxCollisionShape position=\"145,21,71\" halfExtents=\"10,10,1\" info=\"sidearmR, HOOKBOX\"/> <BoxCollisionShape position=\"145,21,29\" halfExtents=\"10,10,1\" info=\"sidearmR, HOOKBOX\"/> <BoxCollisionShape position=\"145,32,50\" halfExtents=\"10,1,20\" info=\"sidearmR, HOOKBOX\"/> <BoxCollisionShape position=\"145,10,50\" halfExtents=\"10,1,20\" info=\"sidearmR, HOOKBOX\"/> <BoxCollisionShape position=\"156,21,50\" halfExtents=\"1,10,20\" info=\"sidearmR, HOOKBOX\"/> <BoxCollisionShape position=\"134,21,50\" halfExtents=\"1,10,20\" info=\"sidearmR, HOOKBOX\"/> </collisionShapes>")
    print("</Pawn>")
end


function placeSidearmRfront(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=100 initialhealth=100 maxhealth=100 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_sidearmRfront radarname = \"HIDDEN\" >")
    print("<attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_sidearmRfront.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"145,21,50\" halfExtents=\"10,10,20\" info=\"sidearmRfront, HOOK\"/> <BoxCollisionShape position=\"141,15,-53\" roll=\"-15\" halfExtents=\"27,44,62\" info=\"sidearmRfront\"/> <BoxCollisionShape position=\"153,59,-30\" roll=\"-15\" halfExtents=\"4,2,38\" info=\"sidearmRfront, top\"/> <BoxCollisionShape position=\"153,61,-41\" roll=\"-15\" halfExtents=\"4,4,11\" info=\"sidearmRfront, top\"/> <BoxCollisionShape position=\"153,60,-65\" roll=\"-15\" halfExtents=\"4,3,4\" info=\"sidearmRfront, top\"/> <BoxCollisionShape position=\"153,59,-41\" roll=\"-15\" halfExtents=\"8,2,11\" info=\"sidearmRfront, top\"/> </collisionShapes>")
    print("</Pawn>")
end


function placeFrontL(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=100 initialhealth=100 maxhealth=100 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_frontL radarname = \"HIDDEN\" >")
    print(" <attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_frontL.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"-11,18,58\" halfExtents=\"10,10,10\" info=\"HOOK, frontL\"/> <BoxCollisionShape position=\"-80,33,-320\" halfExtents=\"7,11,54\" info=\"frontL\"/> <BoxCollisionShape position=\"-79,31,-399\" halfExtents=\"3,6,26\" info=\"frontL\"/> </collisionShapes>")
    print("</Pawn>")
end


function placeFrontR(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=100 initialhealth=100 maxhealth=100 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_frontR radarname = \"HIDDEN\" >")
    print(" <attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_frontR.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"11,18,58\" halfExtents=\"10,10,10\" info=\"HOOK, frontR\"/> <BoxCollisionShape position=\"80,33,-320\" halfExtents=\"7,11,54\" info=\"frontR\"/> <BoxCollisionShape position=\"79,31,-399\" halfExtents=\"3,6,26\" info=\"frontR\"/> </collisionShapes>")
    print("</Pawn>")
end


function placePartL(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=100 initialhealth=100 maxhealth=100 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_partL radarname = \"HIDDEN\" >")
    print("<attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_partL.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"-11,18,80\" halfExtents=\"10,10,10\" info=\"HOOK, partL\"/> <BoxCollisionShape position=\"-94,33,-179\" halfExtents=\"11,13,45\" info=\"partL\"/> <BoxCollisionShape position=\"-96,33,-230\" halfExtents=\"13,17,22\" info=\"partL\"/> </collisionShapes>")
    print("</Pawn>")
end


function placePartR(team, posX, posY, posZ, dirX, dirY, dirZ)
    print("<Pawn health=100 initialhealth=100 maxhealth=100 ")
        print("position = \"")
            print(posX) print(",")
            print(posY) print(",")
            print(posZ) print("\" ")
        print("direction = \"")
            print(dirX) print(",")
            print(dirY) print(",")
            print(dirZ) print("\" ")
        print("team = ")
            print(team) print(" ")
    print("collisionType=dynamic mass=100 name=HeavyCruiser_partR radarname = \"HIDDEN\" >")
    print(" <attached> <Model position=\"0,0,0\" direction=\"-1,0,0\" mesh=\"HeavyCruiser_partR.mesh\" scale=\"40\" /> </attached> <collisionShapes> <BoxCollisionShape position=\"11,18,80\" halfExtents=\"10,10,10\" info=\"HOOK, partR\"/> <BoxCollisionShape position=\"94,33,-179\" halfExtents=\"11,13,45\" info=\"partR\"/> <BoxCollisionShape position=\"96,33,-230\" halfExtents=\"13,17,22\" info=\"partR\"/> </collisionShapes>")
    print("</Pawn>")
end