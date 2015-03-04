
function createObstacle(position, scene, list)
    local mesh = Mesh()
    mesh:loadCube(1)
    mesh:setMaterial(Material("obstacle"))
    table.insert(list, mesh)
    scene:addEntity(mesh)
    local body = RigidBody(Shape.Box, {1,1,1})
    body:setMass(1)
    body:setMesh(mesh)
    body:create({0,0,0,1},position)
    table.insert(list, body)
    scene:addEntity(body)
    return body
end

function createGizmo(position, scene, list)
    local mesh = Mesh()
    mesh:loadCube(0.9)
    mesh:setMaterial(Material("debug"))
    mesh:setPosition(position)
    table.insert(list, mesh)
    scene:addEntity(mesh)
    return mesh
end

function math.rand(low, high)
    return low + (high - low) * math.random()
end

function math.round(value)
    return math.floor(value + 0.5)
end

function table.getkey(t, value)
  for k,v in pairs(t) do
    if v==value then return k end
  end
  return nil
end

function table.contains(t, value)
    return table.getkey(t, value) ~= nil
end

function mul(v0, v)
    local x = v0[1] * v
    local y = v0[2] * v
    local z = v0[3] * v
    return {x,y,z}
end
