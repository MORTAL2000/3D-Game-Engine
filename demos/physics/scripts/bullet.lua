scene = Scene()

-- materials
walls = nil
floor = nil

-- bullet
list = {}
shot = 0

function onInit()
	scene:require("scripts/bullet.mat")

	walls = Material("wall")
	walls:setValue("texture", "bricks_diffuse")
	walls:setValue("textured", 1)
	floor = Material("floor")
	floor:setValue("texture", "grid")
	floor:setValue("textured", 1)

	local mesh = Mesh()
	mesh:loadPlane(35)
	mesh:setMaterial(floor)
	table.insert(list, mesh)
	scene:addEntity(mesh)
	local body = RigidBody(Shape.Box, {35, 0.1, 35})
	body:setMass(0)
	body:setFriction(1.5)
	body:setRestitution(0.2)
	body:setMesh(mesh)
	body:create({0, 0, 0, 1}, {0,0,0})
	table.insert(list, body)
	scene:addEntity(body)

	local spacing = 2.0
	for y = 1, 6, 1 do
		for x = -3, 3, 1 do
			if y % 2 == 0 then
				createCube({x * spacing + 0.5, y * spacing, 0})
			else
				createCube({x * spacing, y * spacing, 0})
			end
		end
	end

	cam = scene:getDefaultCamera()
	cam:setFlightMode()
	cam:setPosition({8, 16, 32})
end

function createCube(pos)
	local mesh = Mesh()
	mesh:loadCube(1)
	mesh:setMaterial(walls)
	table.insert(list, mesh)
	scene:addEntity(mesh)

	local body = RigidBody(Shape.Box, {1, 1, 1})
	body:setMass(1)
	body:setFriction(0.75)
	body:setMesh(mesh)
	body:create({0, 0, 0, 1}, pos)
	table.insert(list, body)
	scene:addEntity(body)
end

function shootSphere()
	local mesh = Mesh("meshes/unitSphere.obj")
	mesh:setScale({2,2,2})
	mesh:setMaterial(walls)

 	local body = RigidBody(Shape.Sphere, 2)
	body:setFriction(0.1)
	body:setRestitution(0.2)
	body:setMass(8)
	body:setMesh(mesh)
	body:create({0, 0, 0, 1}, {0, 10, 20})
	table.insert(list, mesh)
	table.insert(list, body)

	scene:addEntity(mesh)
	scene:addEntity(body)
	body:addImpulse({-1, 0, -370})
end

shot = 0
function onUpdate()
	if isKeyDown("P") and shot == 0 then
		shootSphere()
		shot = 1
	end

	if isKeyDown("Q") then
		quit()
	end
end
