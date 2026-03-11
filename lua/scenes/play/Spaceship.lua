local class = require "class"
local assets = require "assets"
local Bullet = require "scenes.play.Bullet"

local Spaceship = class.new()

function Spaceship:init(game_state)
    self.state = "DEPLOYING"
    self.x = 16
    self.timer = 0
    self.lives = 3
    self.explosion_frames = 0
    self.game_state = game_state
end

function Spaceship:collide_rect(rct, anc)
    if self.state ~= "DEPLOYED" then return false end
    local ship_rct = { '!', x=math.floor(self.x + 0.5), y=216, w=15, h=8, anchor='N' }
    local other_rct = { '!', x=rct.x, y=rct.y, w=rct.w, h=rct.h, anchor=anc }
    return pico.vs.rect_rect(ship_rct, other_rct)
end

function Spaceship:explode()
    pico.output.sound(assets.SFX_SPACESHIP_KILLED)
    self.state = "EXPLODING"
    self.timer = 0
    self.explosion_frames = 0
end

function Spaceship:shoot()
    pico.output.sound(assets.SFX_SPACESHIP_SHOOT)
    return Bullet:new(self.x, 216, -200)
end

function Spaceship:update(delta)
    local view = pico.get.view()
    local size = view.dim
    self.timer = self.timer + delta

    if self.state == "DEPLOYING" then
        if self.timer >= 2.0 then
            self.state = "DEPLOYED"
            self.x = 24
            self.timer = self.timer - 2.0
        end
    elseif self.state == "DEPLOYED" then
        local left = (self.game_state.keys["A"] or self.game_state.keys["Left"]) and 1 or 0
        local right = (self.game_state.keys["D"] or self.game_state.keys["Right"]) and 1 or 0
        
        self.x = self.x - 80 * delta * left
        self.x = self.x + 80 * delta * right
        self.x = math.max(24, math.min(size.w - 24, self.x))
        self.timer = self.timer - delta
    elseif self.state == "EXPLODING" then
        if self.timer >= 0.1 then
            self.timer = self.timer - 0.1
            self.explosion_frames = self.explosion_frames + 1
            if self.explosion_frames >= 10 then
                self.state = "DEPLOYING"
                self.lives = self.lives - 1
            end
        end
    end
end

function Spaceship:draw()
    if self.state == "DEPLOYING" then return end

    local draw_x = math.floor(self.x + 0.5)
    local draw_y = 216
    local key = ""
    local crop = { '!', x=0, y=0, w=16, h=8, anchor='NW' }

    if self.state == "DEPLOYED" then
        key = "/crop/ship/deployed"
        crop.x = 0
    elseif self.state == "EXPLODING" then
        key = "/crop/ship/exploding/" .. (self.explosion_frames % 2)
        crop.x = 16.0 * (1 + (self.explosion_frames % 2))
    end

    pico.layer.image('=', "ship", assets.IMG_SPACESHIP)
    pico.layer.sub('=', key, "ship", crop)
    local dst = { '!', x=draw_x, y=draw_y, w=16, h=8, anchor='N' }
    pico.output.draw.layer(key, dst)
end

return Spaceship
