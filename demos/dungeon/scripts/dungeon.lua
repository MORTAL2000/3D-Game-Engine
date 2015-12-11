scene = Scene()

-- materials
floor = nil
player = nil

-- objects
map = {}
objects = {}
elevators = {}

function onInit()
	scene:require("scripts/dungeon.mat")

	floor = Material("floor")
	generateMap(50, 50, 200, 10)

	camera = scene:getDefaultCamera()
	camera:setFlightMode()
end

function onDraw()
	for key,value in pairs(elevators) do
		debug.drawWireCube({value[1] * 2, 0, value[2] * 2}, {1,1,1})
	end
end

function generateMap(width, height, blocks, chance)
	math.randomseed(os.time())
	for x = 0, width+2, 1 do
		map[x] = {}
		for y = 0, height+2, 1 do
			map[x][y] = 0
		end
	end
	map[1][1] = 1
	max = width * height
	if blocks > max then
		blocks = max
	end

	count = 1
	x = 0
	y = 1
	pos_x = 1
	pos_y = 1
	lastdir = 1
	while count < blocks do
		randomising = 1

		if count % 50 == 0 then
			print("Elevator here")
			table.insert(elevators, {pos_x, pos_y})
		end

		while randomising == 1 do
			rand = math.random(4+chance)
			if rand == 1 and lastdir ~= 3 then
				x = 0
				y = 1
				lastdir = 1
			elseif rand == 2 and lastdir ~= 4 then
				x = 1
				y = 0
				lastdir = 2
			elseif rand == 3 and lastdir ~= 1 then
				x = 0
				y = -1
				lastdir = 3
			elseif rand == 4 and lastdir ~= 2 then
				x = -1
				y = 0
				lastdir = 4
			elseif rand > 4 then
				x = x
				y = y
			end

			if (pos_x + x) >= 1 and (pos_y + y) >= 1 and (pos_x + x) <= width and (pos_y + y) <= height then
				map[pos_x][pos_y] = 1
				randomising = 0
				pos_x = pos_x + x
				pos_y = pos_y + y
			end
		end

		count = count + 1
	end

	for x = 1, width+1, 1 do
		for y = 1, height+1, 1 do
			if map[x][y] == 1 then
				addGround(x, y)
				addCeiling(x, y)

				if map[x][y+1] == 0 then
					addWall(x, y+1, 0)
				end

				if map[x][y-1] == 0 then
					addWall(x, y-1, 2)
				end

				if map[x-1][y] == 0 then
					addWall(x-1, y, 1)
				end

				if map[x+1][y] == 0 then
					addWall(x+1, y, 3)
				end
			end
		end
	end
end

function addGround(x, y)
	local part = Mesh()
	part:loadPlane(1.0)
	part:setPosition({x*2, -1, y*2})
	part:setMaterial(floor)
	table.insert(objects, part)
	scene:addEntity(part)
end

function addCeiling(x, y)
	local part = Mesh("meshes/ceiling.obj")
	part:setPosition({x*2, 0, y*2})
	part:setMaterial(floor)
	table.insert(objects, part)
	scene:addEntity(part)
end

function addWall(x, y, dir)
	if dir == 0 then
		wall = Mesh("meshes/wall_n.obj")
		wall:setPosition({x*2, 0, y*2})
		wall:setMaterial(floor)
		table.insert(objects, wall)
		scene:addEntity(wall)
	end

	if dir == 2 then
		wall = Mesh("meshes/wall_s.obj")
		wall:setPosition({x*2, 0, y*2})
		wall:setMaterial(floor)
		table.insert(objects, wall)
		scene:addEntity(wall)
	end

	if dir == 1 then
		wall = Mesh("meshes/wall_e.obj")
		wall:setPosition({x*2, 0, y*2})
		wall:setMaterial(floor)
		table.insert(objects, wall)
		scene:addEntity(wall)
	end

	if dir == 3 then
		wall = Mesh("meshes/wall_w.obj")
		wall:setPosition({x*2, 0, y*2})
		wall:setMaterial(floor)
		table.insert(objects, wall)
		scene:addEntity(wall)
	end
end
