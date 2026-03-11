local class = require "class"
local assets = require "assets"

local UFO = class.new()

function UFO:init()
    self.state = "AWAY"
    self.timer = 0
    self.x = 0
    self.vx = 0
    self.score = 0
end

function UFO:collide_rect(rct, anc)
    if self.state ~= "ALIVE" then return false end
    local ufo_rct = { '!', x=math.floor(self.x + 0.5), y=40, w=16, h=8, anchor='N' }
    local other_rct = { '!', x=rct.x, y=rct.y, w=rct.w, h=rct.h, anchor=anc }
    return pico.vs.rect_rect(ufo_rct, other_rct)
end

function UFO:explode()
    self.state = "EXPLODING"
    self.timer = 0
end

function UFO:update(delta)
    local view = pico.get.view()
    local size = view.dim
    self.timer = self.timer + delta

    if self.state == "AWAY" then
        if self.timer >= 10.0 then
            local left = math.random(0, 1) == 1
            self.state = "ALIVE"
            self.x = left and 28 or (size.w - 28)
            self.vx = left and 25 or -25
            self.timer = self.timer - 10.0
        end
    elseif self.state == "ALIVE" then
        self.x = self.x + delta * self.vx
        if 28 > self.x or self.x > size.w - 28 then
            self.state = "AWAY"
            self.timer = 0
        end
    elseif self.state == "EXPLODING" then
        if self.timer >= 1.0 then
            self.state = "DEAD"
            self.score = math.random(1, 3) * 100
            self.timer = self.timer - 1.0
        end
    elseif self.state == "DEAD" then
        if self.timer >= 1.0 then
            self.state = "AWAY"
            self.timer = self.timer - 1.0
        end
    end
end

function UFO:draw()
    if self.state == "AWAY" then return end

    local draw_x = math.floor(self.x + 0.5)
    local draw_y = 40
    local key = ""
    local crop = { '!', x=0, y=0, w=24, h=8, anchor='NW' }

    pico.layer.image('=', "ufo", assets.IMG_UFO)

    if self.state == "ALIVE" then
        key = "/crop/ufo/alive"
        crop.x = 0
        pico.layer.sub('=', key, "ufo", crop)
        local dst = { '!', x=draw_x, y=draw_y, w=24, h=8, anchor='N' }
        pico.output.draw.layer(key, dst)
    elseif self.state == "EXPLODING" then
        key = "/crop/ufo/exploding"
        crop.x = 24.0
        pico.layer.sub('=', key, "ufo", crop)
        local dst = { '!', x=draw_x, y=draw_y, w=24, h=8, anchor='N' }
        pico.output.draw.layer(key, dst)
    elseif self.state == "DEAD" then
        pico.set.color.draw("red")
        local score_buf = tostring(self.score)
        local txt_r = { '!', x=draw_x, y=draw_y, w=0, h=8, anchor='N' }
        pico.output.draw.text(score_buf, txt_r)
    end
end

return UFO
