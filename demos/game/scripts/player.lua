-- @author Alexander Koch
-- @file player.lua
-- @desc Player input and physics

include("scripts/util.lua")
Player = class()

function Player:__init()
	self.position = {0,0,0}
	self.mesh = nil
	self.body = nil
	self.camera = nil
	self.pi_div_180 = math.pi / 180
end

function Player:create(scene, list)
	-- Create cube mesh
	self.mesh = Mesh()
	self.mesh:loadCube(1)
	table.insert(list, self.mesh)
	scene:addEntity(self.mesh)

	-- Create collision shape
	self.body = RigidBody(Shape.Sphere, 1.0)
	self.body:setMass(70)
	self.body:setMesh(self.mesh)
	self.body:setFriction(0.0)
	self.body:create({0,0,0,1}, self.position)
	table.insert(list, self.body)
	scene:addEntity(self.body)

	-- Load camera
	self.camera = scene:getDefaultCamera()
	self.camera:lookAt({0, 0, -5})

	-- Freeze body rotation
	--self.body:setAngularFactor({0, 0, 0})
end

function Player:update()
	self.position = self.body:getPosition()
	self.camera:setPosition({self.position[1], self.position[2] + 5, self.position[3]})

	local angleY = math.clamp(getAxis("MOUSE_Y") * 0.5 * self.pi_div_180, 5.5, 8)
	local angleX = getAxis("MOUSE_X") * 0.5
	local movement = {math.cos(angleX * self.pi_div_180) * 2, 0, math.sin(angleX * self.pi_div_180) * 2}
	local strafe = {math.cos((angleX+90) * self.pi_div_180) * 2, 0, math.sin((angleX+90) * self.pi_div_180) * 2}

	local lx = self.position[1] + movement[1]
	local ly = self.position[2] + -math.sin(angleY) * math.pi + 5
	local lz = self.position[3] + movement[3]
	local look = {lx, ly, lz}
	self.camera:lookAt(look)

	local force = {0, 0, 0}
	if isKeyDown("W") then
		force = add(force, movement)
	end

	if isKeyDown("S") then
		force = sub(force, movement)
	end

	if isKeyDown("A") then
		force = sub(force, strafe)
	end

	if isKeyDown("D") then
		force = add(force, strafe)
	end

	if isKeyDown(32) then
		self.body:addImpulse({0, 115, 0})
	end

	if isKeyDown(340) then
		self.body:addImpulse({0, -115, 0})
	end

	local velocity = 10
	force[1] = force[1] * velocity
	force[2] = self.body:getLinearVelocity()[2]
	force[3] = force[3] * velocity

	self.body:setLinearVelocity(force)
end
