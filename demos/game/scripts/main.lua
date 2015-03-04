include("scripts/sky.lua")
include("scripts/util.lua")
include("scripts/player.lua")

scene = Scene()

sky = nil
ground = nil
box = nil
tree = nil
player = nil
list = {}

function onInit()
	scene:require("scripts/main.mat")
	--scene:load("levels/level0.lvl")

	ground = Material("ground")
	ground:setValue("_texture", "lightmap")
	ground:setValue("textured", 1)

	box = Material("box")
	box:setValue("_texture", "box")
	box:setValue("textured", 1)

	mesh = Mesh("meshes/map4.obj")
	mesh:setMaterial(ground)
	scene:addEntity(mesh)

 	body = RigidBody(Shape.Mesh)
	body:setMesh(mesh)
	body:setFriction(5.0)
	body:setMass(0)
	body:create({0, 0, 0, 1}, {0, 0, 0})
	scene:addEntity(body)

	sky = Sky()
	sky:create(scene)

	player = Player()
	player:create(scene, list)

	--scene:getDefaultCamera():setFlightMode(true)
	scene:enableShadows({2048, 2048})
end

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

	player:update()
end

function onDraw()
	debug.drawWireCube(player.position, {2, 2, 2})
end
