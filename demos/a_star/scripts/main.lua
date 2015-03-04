dofile("scripts/util.lua")
dofile("scripts/grid.lua")
dofile("scripts/pathfinding.lua")

scene = Scene()
list = {}
grid = nil
path = nil

start = {-10, 0, -10}
target = {10, 0, 10}

enemy = nil

function onInit()
    scene:require("scripts/main.mat")

    local mesh = Mesh()
    mesh:loadCube(1)
    mesh:setScale({20, 0.1, 20})
    mesh:setMaterial(Material("ground"))
    table.insert(list, mesh)
    scene:addEntity(mesh)
    local ground = RigidBody(Shape.Box, {20, 0.1, 20})
    ground:setMass(0)
    ground:setMesh(mesh)
    ground:create({0, 0, 0, 1}, {0, 0, 0})
    table.insert(list, ground)
    scene:addEntity(ground)

    cam = scene:getDefaultCamera()
    cam:setPosition({0, 40, -2})

    --createObstacle({0, 10, 0}, scene, list)

    grid = Grid({0, 0, 0}, {40, 40}, 1)
    grid:create(scene, list)
    path = Path(grid)
end

function onUpdate()
    path:find(start, target)
    grid:debugPath(scene, list)

    grid:getNodeFromWorld(start).gizmo:setMaterial(Material("obstacle"))
    grid:getNodeFromWorld(target).gizmo:setMaterial(Material("obstacle"))

    if isKeyDown("W") then
        start[3] = start[3] - 0.3
    end

    if isKeyDown("S") then
        start[3] = start[3] + 0.3
    end

    if isKeyDown("A") then
        start[1] = start[1] - 0.3
    end

    if isKeyDown("D") then
        start[1] = start[1] + 0.3
    end
end
