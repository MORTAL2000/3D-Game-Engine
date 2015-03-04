dofile("scripts/grid.lua")

Path = class()

function Path:__init(grid)
	self.grid = grid
end

function Path:find(start, target)
	local startNode = self.grid:getNodeFromWorld(start)
	local endNode = self.grid:getNodeFromWorld(target)

	local openSet = {}
	local closedSet = {}
	table.insert(openSet, startNode)

	while #openSet > 0 do
		local current = openSet[1]

		for i = 1, #openSet do
			if (openSet[i].fCost < current.fCost) or openSet[i].fCost == current.fCost and openSet[i].hCost < current.hCost then
				current = openSet[i]
			end
		end

		table.remove(openSet, table.getkey(openSet, current))
		table.insert(closedSet, current)

		if current == endNode then
			self:retracePath(startNode, endNode)
			return
		end

		local neighbours = self.grid:getNeighbours(current)
		for n = 1, #neighbours do
			local neighbour = neighbours[n]

			if not (not neighbour.walkable or table.contains(closedSet, neighbour)) then
				local costToNeighbour = current.gCost + self:getDistance(current, neighbour)
				if costToNeighbour < neighbour.gCost or not table.contains(openSet, neighbour) then
					neighbour.gCost = costToNeighbour
					neighbour.hCost = self:getDistance(neighbour, endNode)
					neighbour.parent = current

					if openSet[key] == nil then
						table.insert(openSet, neighbour)
					end
				end
			end
		end
	end

	print(#openSet)
	print(#closedSet)
end

function Path:retracePath(startNode, endNode)
	local path = {}
	local current = endNode

	while current ~= startNode do
		table.insert(path, current)
		current = current.parent
	end

	self.grid.path = path
end

function Path:getDistance(nodeA, nodeB)
	local distX = math.abs(nodeA.position[1] - nodeB.position[1])
	local distY = math.abs(nodeA.position[3] - nodeB.position[3])

	if distX > distY then
		return 14 * distX + 10 * (distX - distY)
	end

	return 14 * distX + 10 * (distY - distX)
end
