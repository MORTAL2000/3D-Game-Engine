-- main.lua
scene = Scene()

-- materials
spherical = nil
matcap = nil

-- mesh
mesh = nil

-- camera
delta = 0
cam = nil

function onInit()
    scene:require("scripts/sh.mat")

    matcap = Material("matcap")
    matcap:setValue("matcap", "matcap0")

    mesh = Mesh("meshes/testmesh.obj")
    mesh:setPosition({0, 0, 0})
    mesh:setScale({10, 10, 10})
    mesh:setMaterial(matcap)
    scene:addEntity(mesh)

	cam = scene:getDefaultCamera()
end


function onUpdate()
	local distance = 40
	local height = 5
	local x = distance*math.cos(delta)
	local z = distance*math.sin(delta)
	local pos = {x, height, z}
	cam:setPosition(pos)
	cam:lookAt({0.0, 0.0, 0.0})
	delta = delta + 0.01
end
