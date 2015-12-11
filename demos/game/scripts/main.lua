-- @author Alexander Koch
-- @file main.lua
-- @desc Demo of a first person game

-- Include some files
include("scripts/sky.lua")
include("scripts/util.lua")
include("scripts/player.lua")

-- Scene info
scene = Scene()

sky = nil
ground = nil
box = nil
tree = nil
player = nil
list = {}

-- Main function (Engine)
function onInit()
	scene:require("scripts/main.mat")

	ground = Material("ground")
	box = Material("box")

	-- Use multiple files for frustum culling
	spawn("meshes/b1.obj", ground)
	spawn("meshes/b2.obj", ground)
	spawn("meshes/b3.obj", ground)
	spawn("meshes/b4.obj", ground)

	sky = Sky()
	sky:create(scene)

	player = Player()
	player:create(scene, list)

	scene:enableShadows({2048, 2048})
end

-- Spawn helper function
function spawn(path, mat)
	local mesh = Mesh(path)
	mesh:setMaterial(mat)
	scene:addEntity(mesh)

 	local body = RigidBody(Shape.Mesh)
	body:setMesh(mesh)
	body:setFriction(5.0)
	body:setMass(0)
	body:create({0, 0, 0, 1}, {0, 0, 0})

	table.insert(list, mesh)
	table.insert(list, body)
	scene:addEntity(body)
	return body
end

-- Update function (Engine)
function onUpdate()
	local dir = sky:update()
	scene:renderShadows(dir)
	scene:passShadowInfo(ground)
	scene:passShadowInfo(box)
	ground:setValue("sunDirection", dir[1], dir[2], dir[3])
	box:setValue("sunDirection", dir[1], dir[2], dir[3])

	if isKeyDown("I") then
		createObstacle({0, 20, 0}, scene, list)
	end

	if isKeyDown("P") then
		createObstacle(player.position, scene, list)
	end

	if isKeyDown("C") then
		sky:toggleCycle()
	end

	player:update(delta)
end

-- Debug draw function (Engine)
function onDraw()
	debug.drawWireCube(player.position, {2, 2, 2})
end
