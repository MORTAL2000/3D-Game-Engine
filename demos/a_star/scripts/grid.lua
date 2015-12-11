dofile("scripts/node.lua")
dofile("scripts/util.lua")

Grid = class()

function Grid:__init(position, world_size, node_radius)
    self.position = position
    self.world_size = world_size
    self.node_radius = node_radius
    self.node_diameter = node_radius*2
    self.grid_size_x = math.round(world_size[1] / self.node_diameter)
    self.grid_size_y = math.round(world_size[2] / self.node_diameter)
    self.grid = {}
    self.path = {}
    return t
end

function Grid:create(scene, list)
    local right = {-1, 0, 0}
    local forward = {0, 0, -1}
    local world_bottom_left = sub(sub(self.position, mul(right, self.world_size[1] / 2)), mul(forward, self.world_size[2] / 2))

    for i = 0, self.grid_size_x-1 do
        self.grid[i] = {}

        for j = 0, self.grid_size_y-1 do
            local world_point = add(world_bottom_left, mul(right, i * self.node_diameter + self.node_radius))
            world_point = add(world_point, mul(forward, j * self.node_diameter + self.node_radius))


            local walkable = true --physics.checkSphere(self.node_radius, world_point) --physics.checkSphere(world_point, node_radius, layer_mask)
            if j == 8 and (i == 5 or i == 6 or i == 7) then
                walkable = false
            end

            if j == 11 and (i == 9 or i == 10 or i == 11) then
                walkable = false
            end

            local node = Node(walkable, world_point, i, j)
            self.grid[i][j] = node

            local gizmo = createGizmo(world_point, scene, list)
            if not walkable then
                gizmo:setMaterial(Material("obstacle"))
            end

            node:setGizmo(gizmo)
        end
    end
end

function Grid:getNeighbours(node)
    local neighbours = {}

    local x = -1
    while x <= 1 do

        local y = -1
        while y <= 1 do
            if not (x == 0 and y == 0) then
                local check_x = node.grid_x + x
                local check_y = node.grid_y + y

                if (check_x >= 0 and check_x < self.grid_size_x) and (check_y >= 0 and check_y < self.grid_size_y) then
                    table.insert(neighbours, self.grid[check_x][check_y])
                end
            end

            y = y + 1
        end

        x = x + 1
    end
    return neighbours
end

function Grid:getNodeFromWorld(position)
    local percent_x = (position[1] + self.world_size[1] / 2) / self.world_size[1]
    local percent_y = (position[3] + self.world_size[2] / 2) / self.world_size[2]

    percent_x = math.clamp(percent_x, 0, 1)
    percent_y = math.clamp(percent_y, 0, 1)

    local x = math.round((self.grid_size_x - 1) * percent_x)
    local y = math.round((self.grid_size_y - 1) * percent_y)

    return self.grid[x][y]
end

function Grid:debugPath(scene, list)
    for i = 0, self.grid_size_x-1 do
        for j = 0, self.grid_size_y-1 do
            local node = self.grid[i][j]
            if node.walkable then
                node.gizmo:setMaterial(Material("debug"))
            end
        end
    end

    for i = 1, #self.path do
        local node = self.path[i]
        node.gizmo:setMaterial(Material("ground"))
    end
end
