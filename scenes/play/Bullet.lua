local class = require "class"
local assets = require "assets"

local Bullet = class.new()

function Bullet:init(x, y, vy)
    self.state = "ALIVE"
    self.x = x
    self.y = y
    self.vy = vy
    self.show_explosion = true
end

function Bullet:get_rect()
    return { x = math.floor(self.x + 0.5), y = math.floor(self.y + 0.5), w = 1, h = 7 }
end

function Bullet:die(time)
    self.state = "EXPLODING"
    self.timer = time
    self.show_explosion = false
end

function Bullet:explode(time)
    self:die(time)
    self.show_explosion = true
end

function Bullet:update(delta)
    if self.state == "ALIVE" then
        self.y = self.y + delta * self.vy
        if self.y <= 38 or self.y >= 232 then
            if self.y <= 38 then self.y = 38 else self.y = 232 end
            self:explode(0.3)
        end
    elseif self.state == "EXPLODING" then
        self.timer = self.timer - delta
        if self.timer <= 0 then
            self.state = "DEAD"
        end
    end
end

function Bullet:draw()
    if self.state == "DEAD" then return end

    local draw_x = math.floor(self.x + 0.5)
    local draw_y = math.floor(self.y + 0.5)
    local explosion_img = assets.IMG_EXP3
    if self.vy < 0 then explosion_img = assets.IMG_EXP2 end

    if self.state == "ALIVE" then
        pico.set.color.draw("white")
        local r = { '!', x=draw_x, y=draw_y, w=1, h=7, anchor='C' }
        pico.output.draw.rect(r)
    elseif self.state == "EXPLODING" then
        if self.show_explosion then
            local r = { '!', x=draw_x, y=draw_y, w=0, h=0, anchor='C' }
            pico.output.draw.image(explosion_img, r)
        end
    end
end

return Bullet
