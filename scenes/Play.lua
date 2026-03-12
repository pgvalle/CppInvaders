local class = require "class"
local assets = require "assets"
local UFO = require "scenes.play.UFO"
local Horde = require "scenes.play.Horde"
local Spaceship = require "scenes.play.Spaceship"
local Shield = require "scenes.play.Shield"

local PlayScene = class.new()

function PlayScene:init(game_state)
    self.game_state = game_state
    self.state = "STARTING"
    self.horde_b = nil
    self.ship_b = nil

    self.shields = {}
    for i = 0, 3 do
        local s = Shield:new()
        s:deploy(32 + i * 48)
        table.insert(self.shields, s)
    end

    self.ufo = UFO:new()
    self.horde = Horde:new()
    self.ship = Spaceship:new(game_state)
    self.timer = 0
end

function PlayScene:process_event(event)
    if event.tag == 'key.dn' then
        if event.key == 'Space' then
            if not self.ship_b and self.state == "PLAYING" and self.ship.state == "DEPLOYED" then
                self.ship_b = self.ship:shoot()
            end
        elseif event.key == 'Escape' then
            local PauseScene = require "scenes.Pause"
            self.game_state.set_scene(PauseScene:new(self.game_state, self))
        end
    end
end

function PlayScene:process_collisions()
    local b_anc = 'C'
    if self.ship_b and self.ship_b.state == "ALIVE" and self.horde_b and self.horde_b.state == "ALIVE" then
        local sr = self.ship_b:get_rect()
        local hr = self.horde_b:get_rect()
        
        local x_min = math.max(sr.x, hr.x)
        local x_max = math.min(sr.x + sr.w, hr.x + hr.w)
        local y_min = math.max(sr.y, hr.y)
        local y_max = math.min(sr.y + sr.h, hr.y + hr.h)
        
        if x_min < x_max and y_min < y_max then
            self.ship_b:explode(0.1)
            self.horde_b:explode(0.1)
        end
    end

    if self.ship_b and self.ship_b.state == "ALIVE" then
        local sr = self.ship_b:get_rect()

        if self.ufo:collide_rect(sr, b_anc) then
            self.ufo:explode()
            self.ship_b:die(0.3)
        end

        local i = self.horde:collide_rect(sr, b_anc)
        if i >= 0 then
            local score = self.horde:kill_invader(i)
            self.ship_b:die(0.3)
            self.game_state.add_to_score(score)
        end

        for _, s in ipairs(self.shields) do
            if s:damage_shot(self.ship_b) then
                self.ship_b:explode(0.1)
                break
            end
        end
    end

    if self.horde_b and self.horde_b.state == "ALIVE" then
        local hr = self.horde_b:get_rect()
        if self.ship:collide_rect(hr, b_anc) then
            self.ship:explode()
            self.horde_b:die(0.3)
        end
        
        for _, s in ipairs(self.shields) do
            if s:damage_shot(self.horde_b) then
                self.horde_b:explode(0.1)
                break
            end
        end
    end

    for i = 1, 55 do
        local inv = self.horde.invaders[i]
        if inv and inv:is_alive() then
            local inv_rct = inv:get_rect()
            for _, s in ipairs(self.shields) do
                s:damage_rect(inv_rct)
            end
        end
    end
end

function PlayScene:update(delta)
    if self.state == "STARTING" then
        self.horde:update(delta)
        self.ship:update(delta)
        if self.horde.state == "MARCHING" then
            self.state = "PLAYING"
        end
    elseif self.state == "PLAYING" then
        if self.horde_b then
            self.horde_b:update(delta)
            if self.horde_b.state == "DEAD" then
                self.horde_b = nil
            end
        elseif self.horde.invaders_alive > 0 and self.ship.state == "DEPLOYED" then
            self.horde_b = self.horde:shoot(self.ship.x)
        end

        if self.ship_b then
            self.ship_b:update(delta)
            if self.ship_b.state == "DEAD" then
                self.ship_b = nil
            end
        end

        self:process_collisions()

        self.ufo:update(delta)
        self.horde:update(delta)
        self.ship:update(delta)

        if self.horde.invaders_alive == 0 or self.ship.lives == 0 then
            self.state = "FIN"
            self.timer = 0
            self.horde_b = nil
            self.ship_b = nil
        end
    elseif self.state == "FIN" then
        self.timer = self.timer + delta
        if self.timer >= 2 and self.ship.lives == 0 then
            local OverScene = require "scenes.Over"
            self.game_state.set_scene(OverScene:new(self.game_state, self))
        elseif self.timer >= 2 then
            self.state = "STARTING"
            self.ufo = UFO:new()
            self.horde = Horde:new()
            self.ship = Spaceship:new(self.game_state)
            for _, s in ipairs(self.shields) do
                s:deploy(s.x)
            end
        end
    end
end

function PlayScene:draw()
    self.ufo:draw()
    self.horde:draw()
    for _, s in ipairs(self.shields) do s:draw() end
    if self.horde_b then self.horde_b:draw() end
    if self.ship_b then self.ship_b:draw() end
    self.ship:draw()

    pico.set.color.draw("white")
    
    local lives_buf = tostring(self.ship.lives)
    local lives_txt_r = { '!', x=8.0, y=248.0, w=0, h=8, anchor='SW' }
    pico.output.draw.text(lives_buf, lives_txt_r)

    local ship_x = 24.0
    for i = 1, self.ship.lives - 1 do
        local crop = { '!', x=0, y=0, w=16, h=8, anchor='NW' }
        pico.layer.image('=', "ship", assets.IMG_SPACESHIP)
        pico.layer.sub('=', "/crop/ship/life", "ship", crop)
        local dst = { '!', x=ship_x, y=248.0, w=16, h=8, anchor='SW' }
        pico.output.draw.layer("/crop/ship/life", dst)
        ship_x = ship_x + 16.0
    end
end

return PlayScene
