function asteroidField(posX,posY,posZ,radius,count)
	for i = 1, count, 1
	do
		size = math.random()
		print("<MovableEntity ")
		
		print("position = \"") 
		print((2 * math.random() * radius) - radius + posX) print(",") 
		print((2 * math.random() * radius) - radius + posY) print(",") 
		print((2 * math.random() * radius) - radius + posZ) print("\"")
		
		print("scale = \"") print(size * 150) print("\"")
		
		print("collisionType = dynamic linearDamping = 0.8 angularDamping = 0 collisiondamage = 1 enablecollisiondamage = true>")
		print("<attached> <Model mass=\"") print(size * 1000) print("\" mesh=\"ast") print(math.mod(i,6) + 1) print(".mesh\" /> </attached>")
		print("<collisionShapes> <SphereCollisionShape radius=\"") print(size * 350) print("\" /> </collisionShapes>")
		print("</MovableEntity>")
	end
	
end
