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
		pX = (2 * math.random() * radius) - radius + posX
		pY = (2 * math.random() * radius) - radius + posY
		pZ = (2 * math.random() * radius) - radius + posZ
		print("<StaticEntity ")
		
		print("position = \"") 
		print(pX) print(",") 
		print(pY) print(",") 
		print(pZ) print("\" ")
		
		print("scale = \"") print(size) print("\" ")
		
		print("collisionType = static linearDamping = 0.8 angularDamping = 1 ") 
		print("collisiondamage = 1000 enablecollisiondamage = true>")
		
		print("<attached>")
			print("<Model mass=\"") print(size * 10) print("\" ")
			print("mesh=\"ast") print(math.mod(i,6) + 1) print(".mesh\" />")
		print("</attached>")
		
		print("<collisionShapes> ")
			print("<SphereCollisionShape radius=\"") 
			print(size * 2.5) print("\" />")
		print("</collisionShapes>")
		
		print("</StaticEntity>")
	end
end


--[[ asteroid belt generator
generates asteroid belt
	posX, posY, posZ - position in space
	yaw, pitch - rotation
	minSize, maxSize - size boundaries of each asteroid
	radius0, radius1 - inner/outer radius
	count - number of asteroids
--]]
function asteroidBelt(centerX, centerY, centerZ, yaw, pitch, segments, minSize, maxSize, radius0, radius1, count)
	dPhi = (2 * math.pi) / segments
	width = math.abs(radius1 - radius0)
	radius = (radius1 + radius0) / 2
	segmentCount = count / segments
	
	print("<StaticEntity collisionType=static yaw=") print(yaw) 
	print(" pitch=") print(pitch)
	
	print(" position = \"") 
		print(centerX) print(",") 
		print(centerY) print(",") 
		print(centerZ) print("\"") 
	print(">")
	
	print("<attached>")
	
	for i = 0, segments - 1, 1
	do
		asteroidField((radius * math.cos(i * dPhi)),
					(radius * math.sin(i * dPhi)),
					0, minSize, maxSize, width,segmentCount)
	end
	
	print("</attached>")
	print("</StaticEntity>")
end
