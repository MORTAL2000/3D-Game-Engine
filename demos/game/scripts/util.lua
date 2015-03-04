
function createObstacle(position, scene, list)
	local mesh = Mesh("meshes/box.obj")
	mesh:setMaterial(Material("box"))
	mesh:setScale({1.2,1.2,1.2})
	table.insert(list, mesh)
	scene:addEntity(mesh)
	local body = RigidBody(Shape.Box, {1.2,1.2,1.2})
	body:setMass(1)
	body:setMesh(mesh)
	body:setFriction(16.0)
	body:create({0,0,0,1},position)
	table.insert(list, body)
	scene:addEntity(body)
	return body
end
