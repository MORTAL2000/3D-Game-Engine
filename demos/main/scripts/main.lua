-- main.lua
dofile("scripts/sky.lua")
scene = Scene()

sky = nil
ground = nil

function onInit()
    scene:require("scripts/main.mat")

    ground = Material("ground")
    mesh = Mesh("meshes/simplex.obj")
    mesh:setMaterial(ground)
    scene:addEntity(mesh)

    sky = Sky()
    sky:create(scene)

    local camera = scene:getDefaultCamera()
    camera:setPosition({8, 16, 32})
    camera:setFlightMode()
end

function onUpdate()
    local dir = sky:update()
    ground:setValue("sunDirection", dir[1], dir[2], dir[3])
end
