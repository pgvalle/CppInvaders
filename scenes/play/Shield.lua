local class = require "class"
local assets = require "assets"
local Bullet = require "scenes.play.Bullet"

local Shield = class.new()

local damage_pattern_1 = {
                   {-1,0},
    {-3,1},                       {1,1},
                   {-1,2}, {0,2},        {2,2},
           {-2,3}, {-1,3}, {0,3}, {1,3},
    {-3,4},        {-1,4}, {0,4}, {1,4},
           {-2,5}, {-1,5}, {0,5}, {1,5}, {2,5},
    {-3,6},        {-1,6}, {0,6}, {1,6},
           {-2,7},         {0,7},        {2,7},
}

local damage_pattern_2 = {
    {-3,0},                        {1,0},               {4,0},
                    {-1,1},                      {3,1},
            {-2,2}, {-1,2}, {0,2}, {1,2}, {2,2}, {3,2},
    {-3,3}, {-2,3}, {-1,3}, {0,3}, {1,3}, {2,3}, {3,3}, {4,3},
    {-3,4}, {-2,4}, {-1,4}, {0,4}, {1,4}, {2,4}, {3,4}, {4,4},
            {-2,5}, {-1,5}, {0,5}, {1,5}, {2,5}, {3,5},
                    {-1,6},               {2,6},
    {-3,7},                 {0,7},                      {4,7},
}

function Shield:deploy(x)
    self.x = x
    self.bits = {}
    for i = 0, 351 do self.bits[i] = true end

    for i = 0, 3 do
        for j = 0, 3 - i do
            self.bits[22 * i + j] = false
            self.bits[22 * i - j + 21] = false
        end
        
        local l = 15 - i
        for j = 5 + i, 17 - i do
            self.bits[22 * l + j] = false
            self.bits[22 * l - j + 21] = false
        end
    end
end

function Shield:damage_shot(shot)
    local shotx = math.floor(shot.x + 0.5)
    local shoty = math.floor(shot.y + 0.5)
    local point_count = 0

    for y = 2, 4 do
        local relx = shotx - self.x
        local rely = shoty - 192 + y
        if relx >= 0 and relx < 22 and rely >= 0 and rely < 16 then
            if self.bits[22 * rely + relx] then
                point_count = point_count + 1
            end
        end
    end

    if point_count < 1 then return false end

    local pat = damage_pattern_2
    if shot.vy >= 0 then pat = damage_pattern_1 end
    
    for _, off in ipairs(pat) do
        local relx = shotx - self.x + off[1]
        local rely = shoty - 192 + off[2]
        if relx >= 0 and relx < 22 and rely >= 0 and rely < 16 then
            self.bits[22 * rely + relx] = false
        end
    end

    return true
end

function Shield:damage_rect(rect)
    local r1 = {x=rect.x, y=rect.y, w=rect.w, h=rect.h}
    local r2 = {x=self.x, y=192, w=22, h=16}
    
    local x_min = math.max(r1.x, r2.x)
    local x_max = math.min(r1.x + r1.w, r2.x + r2.w)
    local y_min = math.max(r1.y, r2.y)
    local y_max = math.min(r1.y + r1.h, r2.y + r2.h)
    
    if x_min < x_max and y_min < y_max then
        for yi = 0, (y_max - y_min) - 1 do
            for xi = 0, (x_max - x_min) - 1 do
                local relx = xi + x_min - self.x
                local rely = yi + y_min - 192
                self.bits[22 * rely + relx] = false
            end
        end
    end
end

function Shield:draw()
    local points = {}
    for i = 0, 351 do
        if self.bits[i] then
            local px = self.x + (i % 22)
            local py = 192 + math.floor(i / 22)
            table.insert(points, { '!', x=px, y=py, anchor='NW' })
        end
    end

    pico.set.color.draw("green")
    
    local batch_size = 32
    for i = 1, #points, batch_size do
        local batch = {}
        for j = i, math.min(i + batch_size - 1, #points) do
            table.insert(batch, points[j])
        end
        pico.output.draw.pixels(batch)
    end
end

return Shield
