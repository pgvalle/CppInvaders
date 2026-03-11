local class = require "class"
local assets = require "assets"

local Invader = class.new()
Invader.dx = 2
Invader.dy = 0

function Invader:init(i)
    local row = 4 - math.floor(i / 11)
    local col = i % 11

    self.state = "DOWN"
    self.type = math.floor(row / 2)
    self.x = 32 + 16 * col
    self.y = 64 + 16 * row
end

function Invader:get_rect()
    if self.type == 0 then return {x=self.x, y=self.y, w=8, h=8}
    elseif self.type == 1 then return {x=self.x, y=self.y, w=11, h=8}
    else return {x=self.x, y=self.y, w=12, h=8} end
end

function Invader:get_image()
    if self.type == 0 then return assets.IMG_INV1
    elseif self.type == 1 then return assets.IMG_INV2
    else return assets.IMG_INV3 end
end

function Invader:is_alive()
    return self.state == "UP" or self.state == "DOWN"
end

function Invader:kill()
    self.state = "DYING"
    self.timer = 0
end

function Invader:update(delta)
    if self.state == "DOWN" then
        self.state = "UP"
        self.x = self.x + Invader.dx
        self.y = self.y + Invader.dy
    elseif self.state == "UP" then
        self.state = "DOWN"
        self.x = self.x + Invader.dx
        self.y = self.y + Invader.dy
    elseif self.state == "DYING" then
        self.timer = self.timer + delta
        if self.timer >= 0.5 then
            self.state = "DEAD"
        end
    end
end

function Invader:draw()
    if self.state == "DEAD" then return end

    local img = self:get_image()
    local r = self:get_rect()
    local draw_x = self.x
    local draw_y = self.y
    local w = r.w
    local h = r.h

    local key = ""
    local crop = { '!', x=0, y=0, w=w, h=h, anchor='NW' }

    if self.state == "UP" then
        key = "/crop/invader/" .. self.type .. "/up"
        crop.x = w
        pico.layer.image('=', img, img)
        pico.layer.sub('=', key, img, crop)
    elseif self.state == "DOWN" then
        key = "/crop/invader/" .. self.type .. "/down"
        crop.x = 0
        pico.layer.image('=', img, img)
        pico.layer.sub('=', key, img, crop)
    elseif self.state == "DYING" then
        key = "/crop/invader/dying"
        img = assets.IMG_EXP1
        w = 16.0; h = 8.0
        crop.w = w; crop.h = h
        pico.layer.image('=', img, img)
        pico.layer.sub('=', key, img, crop)
    else
        return
    end

    local dst = { '!', x=draw_x, y=draw_y, w=w, h=h, anchor='N' }
    pico.output.draw.layer(key, dst)
end

return Invader
