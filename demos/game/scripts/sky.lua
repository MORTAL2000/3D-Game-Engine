-- @author Alexander Koch
-- @file sky.lua
-- @desc Simulates a sky using a skybox and a shader.

Sky = class()

function Sky:__init()
	self.material = Material("sky")
	self.skybox = Mesh()
	self.elevationAngle = 2.45
	self.azimuthAngle = -27.7
	self.cycle = false
	self.delta = 0
end

function Sky:create(scene)
	self.skybox:loadCube(500)
	self.skybox:setCullFace("front")
	self.skybox:setUseDepthMask(false)
	self.skybox:setMaterial(self.material)
	scene:addEntity(self.skybox)
end

function Sky:toggleCycle()
	self.cycle = not self.cycle
end

function Sky:mulvec4(mat, v0, v1, v2, v3)
	local r0 = mat:get(0,0) * v0 + mat:get(0,1) * v1 + mat:get(0,2) * v2 + mat:get(0,3) * v3
	local r1 = mat:get(1,0) * v0 + mat:get(1,1) * v1 + mat:get(1,2) * v2 + mat:get(1,3) * v3
	local r2 = mat:get(2,0) * v0 + mat:get(2,1) * v1 + mat:get(2,2) * v2 + mat:get(2,3) * v3
	return {r0, r1, r2}
end

function Sky:mul(v0, v1)
	v2 = {0, 0, 0}
	v2[1] = v0[1] * v1[1]
	v2[2] = v0[2] * v1[2]
	v2[3] = v0[3] * v1[3]
	return v2
end

function Sky:update()
	local elevation = matrix:rotation(self.elevationAngle, {0, 0, 1})
	local rotation = matrix:rotation(self.azimuthAngle, {0, 1, 0})
	local vector = self:mul(self:mulvec4(rotation * elevation, 1, 0, 0, 1), {1, -1, -1})
	self.material:setValue("sunDirection", vector[1], vector[2], vector[3])
	self.material:setValue("delta", self.delta)

	if self.cycle then
		self.elevationAngle = self.elevationAngle - 0.0005
		self.azimuthAngle = self.azimuthAngle + 0.00005
	end

	self.delta = self.delta + 1
	return vector
end
