local class = require "class"

local OverScene = class.new()
local OVER_STRING = "GAME OVER"

function OverScene:init(game_state, gameplay)
    self.game_state = game_state
    self.gameplay = gameplay
    self.waiting = false
    self.timer = 0
    self.ci = 0
end

function OverScene:process_event(event)
    if event.tag == 'key.dn' and event.key == 'Return' then
        if not self.waiting then
            self.waiting = true
            self.timer = 0
            self.ci = string.len(OVER_STRING)
        else
            self.timer = 2.5
        end
    end
end

function OverScene:update(delta)
    self.timer = self.timer + delta

    if not self.waiting and self.timer >= 0.2 then
        self.ci = self.ci + 1
        if self.ci >= string.len(OVER_STRING) then
            self.waiting = true
        end
        self.timer = self.timer - 0.2
        return
    end

    if self.waiting and self.timer >= 2.5 then
        local SplashScene = require "scenes.Splash"
        self.game_state.set_scene(SplashScene:new(self.game_state))
        return
    end
end

function OverScene:draw()
    if self.gameplay then
        self.gameplay:draw()
    end

    pico.push()
    pico.set.color.draw({r=0, g=0, b=0})
    pico.set.alpha(204)
    local dim_r = { '%', x=0.0, y=0.0, w=1.0, h=1.0, anchor='NW' }
    pico.output.draw.rect(dim_r)
    pico.pop()

    local str = string.sub(OVER_STRING, 1, self.ci)
    local cnt_r = { '%', x=0.5, y=0.5, w=0, h=0, anchor='C' }
    if str ~= "" then
        pico.set.color.draw("red")
        local txt_r = { '#', x=0.0, y=0.0, w=0.0, h=1.0, anchor='C', up=cnt_r }
        pico.output.draw.text(str, txt_r)
    end
end

return OverScene
