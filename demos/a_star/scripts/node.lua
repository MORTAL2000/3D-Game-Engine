Node = class()

function Node:__init(walkable, position, x, y)
    self.walkable = walkable
    self.position = position
    self.grid_x = x
    self.grid_y = y
    self.fCost = 0
    self.gCost = 0
    self.hCost = 0
    self.parent = nil
end

function Node:setGizmo(gizmo)
    self.gizmo = gizmo
end
