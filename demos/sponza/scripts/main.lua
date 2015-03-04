scene = Scene()

function onInit()
	scene:require("scripts/sponza.mat")

	local mat = Material("diffuse")
	mat:setValue("texture", "lightmap")
	mat:setValue("textured", 1)

	mesh = Mesh("meshes/sponza.obj")
	mesh:setMaterial(mat)
	scene:addEntity(mesh)

	env = Mesh()
	env:loadCube(500)
	env:setCullFace("front")
	env:setUseDepthMask(false)
	env:setMaterial(Material("sky"))
	scene:addEntity(env)

	cam = scene:getDefaultCamera()
	cam:setPosition({-16, 3, 0})
	cam:setFlightMode()
end

local shot =0 
function onUpdate()
	cam:lookAt({0, 5, 0})
end
