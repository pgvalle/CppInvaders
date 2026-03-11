local class = require "class"
local assets = require "assets"
local Invader = require "scenes.play.Invader"
local Bullet = require "scenes.play.Bullet"

local Horde = class.new()
local sfx_table = { 
    assets.SFX_INVADER_STEP0, 
    assets.SFX_INVADER_STEP1, 
    assets.SFX_INVADER_STEP2, 
    assets.SFX_INVADER_STEP3 
}

function Horde:init()
    self.state = "DEPLOYING"
    self.invaders_alive = 0
    self.timer = 0
    self.i = 0 
    self.sfx_i = 1 
    self.sfx_timer = 0
    self.invaders = {}
end

function Horde:collide_rect(rct, anc)
    local other_rct = { '!', x=rct.x, y=rct.y, w=rct.w, h=rct.h, anchor=anc }
    for i = 0, 54 do
        local inv = self.invaders[i + 1]
        if inv and inv:is_alive() then
            local ir = inv:get_rect()
            local inv_rct = { '!', x=ir.x, y=ir.y, w=ir.w, h=ir.h, anchor='N' }
            if pico.vs.rect_rect(other_rct, inv_rct) then
                return i
            end
        end
    end
    return -1
end

function Horde:kill_invader(i)
    pico.output.sound(assets.SFX_INVADER_KILLED)
    local inv = self.invaders[i + 1]
    inv:kill()
    self.invaders_alive = self.invaders_alive - 1

    local score = 10 * (3 - inv.type)
    return score
end

function Horde:shoot(ship_x)
    local pos = {x=0, y=0}

    for i = 1, 55 do
        local inv = self.invaders[i]
        if inv then
            local above_ship = math.abs(ship_x - inv.x) <= 8
            if inv.y > pos.y and above_ship and inv:is_alive() then
                pos = {x=inv.x, y=inv.y}
            end
        end
    end

    if math.random(0, 2) ~= 0 and pos.y > 0 then
        return Bullet:new(pos.x, pos.y + 12, 120)
    end

    local a = {}
    for i = 54, 33, -1 do
        local inv = self.invaders[i + 1]
        if inv and inv:is_alive() then
            table.insert(a, i)
        end
    end
    
    if #a > 0 then
        local ri = math.random(1, #a)
        local sel_inv = self.invaders[a[ri] + 1]
        pos = {x=sel_inv.x, y=0}

        for i = 55, 1, -1 do
            local inv = self.invaders[i]
            if inv then
                local same_column = math.abs(inv.x - pos.x) <= 8
                if inv.y > pos.y and same_column and inv:is_alive() then
                    pos = {x=inv.x, y=inv.y}
                end
            end
        end
    end

    return Bullet:new(pos.x, pos.y + 12, 120)
end

function Horde:update(delta)
    local view = pico.get.view()
    local size = view.dim
    self.timer = self.timer + delta

    if self.state == "DEPLOYING" then
        while self.timer >= 0.016 and self.i < 55 do
            self.invaders[self.i + 1] = Invader:new(self.i)
            self.i = self.i + 1
            self.invaders_alive = self.invaders_alive + 1
            self.timer = self.timer - 0.016
            if self.i == 55 then
                self.state = "MARCHING"
                self.i = 0
                self.sfx_i = 1
                self.sfx_timer = 0
                Invader.dx = 2
                Invader.dy = 0
                break
            end
        end
    elseif self.state == "MARCHING" then
        for i = 1, 55 do
            local inv = self.invaders[i]
            if inv and inv.state == "DYING" then
                inv:update(delta)
            end
        end

        while self.timer >= 0.016 do
            while self.i < 55 and not self.invaders[self.i + 1]:is_alive() do
                self.i = self.i + 1
            end

            if self.i == 55 then
                self.i = 0
                while self.i < 55 and not self.invaders[self.i + 1]:is_alive() do
                    self.i = self.i + 1
                end

                Invader.dy = 0
                for i = 1, 55 do
                    local inv = self.invaders[i]
                    if inv then
                        local out = inv.x < 16 or inv.x >= size.w - 16
                        if inv:is_alive() and out then
                            Invader.dx = -Invader.dx
                            Invader.dy = 8
                            break
                        end
                    end
                end
            end

            if self.i < 55 then
                self.invaders[self.i + 1]:update(delta)
                self.i = self.i + 1
            end

            self.timer = self.timer - 0.016
        end

        self.sfx_timer = self.sfx_timer + delta
        if self.sfx_timer >= 0.016 * self.invaders_alive + 0.1 then
            pico.output.sound(sfx_table[self.sfx_i])
            self.sfx_i = (self.sfx_i % 4) + 1
            self.sfx_timer = 0
        end
    end
end

function Horde:draw()
    for i = 1, 55 do
        if self.invaders[i] then self.invaders[i]:draw() end
    end
end

return Horde
