scene = Scene()

diffuse = nil
camera = nil

velocity = 10
chassis = nil
tire0 = nil
tire1 = nil
tire2 = nil
tire3 = nil
car = nil

function onInit()
	scene:require("scripts/main.mat")
	diffuse = Material("diffuse")

	-- create ground
	ground = Mesh()
	ground:loadPlane(35)
	ground:setMaterial(diffuse)
	scene:addEntity(ground)
	body0 = RigidBody(Shape.Box, {35, 0.1, 35})
	body0:setMass(0)
	body0:setFriction(1.5)
	body0:setRestitution(0.2)
	body0:setMesh(ground)
	body0:create({0, 0, 0, 1}, {0,0,0})
	scene:addEntity(body0)

	--createCar()
	cube()

	cam = scene:getDefaultCamera()
	cam:setPosition({8, 16, 32})
end

function cube()
	chassis = Mesh()
	chassis:loadCube(1)
	chassis:setMaterial(diffuse)

	car = RigidBody(Shape.Box, {1, 1, 1})
	car:setMass(1)
	car:setFriction(0.1)
	car:setRestitution(0.2)
	car:setMesh(chassis)
	car:create({0, 0, 0, 1}, {0, 1, 0})
	scene:addEntity(chassis)
	scene:addEntity(car)
end

function createCar()
	local th = 0.5
	local ts = 1.3

	chassis = Mesh("meshes/chassis.obj")
	chassis:setPosition({0, 0, 0})
	chassis:setMaterial(diffuse)

	tire0 = Mesh("meshes/tire.obj")
	tire0:setPosition({ts, th, 2})
	tire0:setMaterial(diffuse)
	chassis:addChild(tire0)

	tire1 = Mesh("meshes/tire.obj")
	tire1:setPosition({-ts, th, 2})
	tire1:setMaterial(diffuse)
	chassis:addChild(tire1)

	tire2 = Mesh("meshes/tire.obj")
	tire2:setPosition({ts, th, -2})
	tire2:setMaterial(diffuse)
	chassis:addChild(tire2)

	tire3 = Mesh("meshes/tire.obj")
	tire3:setPosition({-ts, th, -2})
	tire3:setMaterial(diffuse)
	chassis:addChild(tire3)

	car = RigidBody(Shape.Box, {1, 1, 2})
	car:setMass(1)
	car:setFriction(0.1)
	car:setRestitution(0.2)
	car:setMesh(chassis)
	car:create({0, 0, 0, 1}, {0, 2, 0})

	scene:addEntity(chassis)
	scene:addEntity(car)
end

function onUpdate()

	if isKeyDown("W") then
		car:addRelativeForce({0, 0, -velocity})
	end

	if isKeyDown("S") then
		car:addRelativeForce({0, 0, velocity})
	end

	if isKeyDown("D") then
		car:addTorque({0, -velocity, 0})
	end
end
