local class = require "class"
local assets = require "assets"

local SplashScene = class.new()
local splash_lines = {
    "PLAY",
    "SPACE INVADERS",
    "*SCORE ADVANCES TABLE*",
    "=? MYSTERY",
    "=30 POINTS",
    "=20 POINTS",
    "=10 POINTS",
    ""
}
local splash_offs = {
    {96, 64}, {56,  88}, {24, 120},  {80, 136},
    {80, 152}, {80, 168}, {80, 184}, { 0,  0}
}

function SplashScene:init(game_state)
    self.game_state = game_state
    self.state = "WAITING1"
    self.timer = 0
    self.li = 1
    self.ci = 0
end

function SplashScene:typewrite_next_char()
    if self.timer < 0.05 then return false end
    
    local line_len = string.len(splash_lines[self.li])
    self.ci = self.ci + 1
    if self.ci > line_len then
        self.li = self.li + 1
        self.ci = 0
    end
    self.timer = self.timer - 0.05
    return true
end

function SplashScene:process_event(event)
    if event.tag ~= 'key.dn' then return end

    if self.state == "WAITING_KEYPRESS" then
        local PlayScene = require "scenes.Play"
        self.game_state.score = 0
        self.game_state.set_scene(PlayScene:new(self.game_state))
    else
        self.state = "WAITING_KEYPRESS"
        self.li = 8
        self.ci = 0
    end
end

function SplashScene:update(delta)
    self.timer = self.timer + delta

    if self.state == "WAITING1" then
        if self.timer >= 1.5 then
            self.state = "TYPEWRITING1"
            self.timer = self.timer - 1.5
        end
    elseif self.state == "TYPEWRITING1" then
        if self:typewrite_next_char() and self.li == 3 then
            self.state = "WAITING2"
        end
    elseif self.state == "WAITING2" then
        if self.timer >= 1.5 then
            self.state = "TYPEWRITING2"
            self.timer = self.timer - 1.5
        end
    elseif self.state == "TYPEWRITING2" then
        if self:typewrite_next_char() and self.li == 8 then
            self.state = "WAITING_KEYPRESS"
        end
    end
end

function SplashScene:draw()
    pico.set.color.draw("white")
    
    for lj = 1, self.li do
        local str
        if lj == self.li then
            str = string.sub(splash_lines[lj], 1, self.ci)
        else
            str = splash_lines[lj]
        end
        if str ~= "" then
            local txt_r = { '!', x=splash_offs[lj][1], y=splash_offs[lj][2], w=0, h=8, anchor='NW' }
            pico.output.draw.text(str, txt_r)
        end
    end

    if self.state == "TYPEWRITING2" or self.state == "WAITING_KEYPRESS" then
        pico.layer.image('=', "ufo", assets.IMG_UFO)
        pico.layer.image('=', "inv1", assets.IMG_INV1)
        pico.layer.image('=', "inv2", assets.IMG_INV2)
        pico.layer.image('=', "inv3", assets.IMG_INV3)

        local function draw_sprite(key_par, name, x, y, sw, sh)
            local key = "/crop/" .. name
            local crop = { '!', x=0, y=0, w=sw, h=sh, anchor='NW' }
            pico.layer.sub('=', key, key_par, crop)
            local dst = { '!', x=x, y=y, w=sw, h=sh, anchor='C' }
            pico.output.draw.layer(key, dst)
        end

        draw_sprite("ufo",  "ufo",  71, 140, 24, 8)
        draw_sprite("inv1", "inv1", 71, 156, 8, 8)
        draw_sprite("inv2", "inv2", 71, 172, 11, 8)
        draw_sprite("inv3", "inv3", 71, 188, 12, 8)
    end
end

return SplashScene
