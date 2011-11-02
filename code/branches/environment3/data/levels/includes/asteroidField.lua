--[[ asteroid field generator
generates asteroid field
	posX, posY, posZ - position in space
	minSize, maxSize - size boundaries of each asteroid
	radius - size of the cube around position in space
	count - number of asteroids
--]]
function asteroidField(posX, posY, posZ, minSize, maxSize, radius, count)
	for i = 1, count, 1
	do
		size = (math.random() * (maxSize - minSize)) + minSize
		print("<StaticEntity ")
		
		print("position = \"") 
		print((2 * math.random() * radius) - radius + posX) print(",") 
		print((2 * math.random() * radius) - radius + posY) print(",") 
		print((2 * math.random() * radius) - radius + posZ) print("\"")
		
		print("scale = \"") print(size) print("\"")
		
		print("collisionType = static linearDamping = 0.8 angularDamping = 0 collisiondamage = 1 enablecollisiondamage = true>")
		print("<attached> <Model mass=\"") print(size * 10) print("\" mesh=\"ast") print(math.mod(i,6) + 1) print(".mesh\" /> </attached>")
		print("<attached> <ForceField position=\"0,0,0\" mode=\"newtonianGravity\" diameter=\"") print(size + (size * 0.2)) print("\" massDiameter=\"") print(size) print("\" mass=\"") print(size * 1000) print("\" /> </attached>")
		print("<collisionShapes> <SphereCollisionShape radius=\"") print(size) print("\" /> </collisionShapes>")
		print("</StaticEntity>")
	end
	
end
