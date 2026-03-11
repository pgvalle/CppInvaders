local class = require "class"

local PauseScene = class.new()

function PauseScene:init(game_state, gameplay)
    self.game_state = game_state
    self.gameplay = gameplay
    self.resuming = false
    self.timer = 0
    self.pause_symbol = true
end

function PauseScene:process_event(event)
    if event.tag == 'key.dn' and event.key == 'Escape' then
        self.resuming = not self.resuming
        self.timer = 0
        self.pause_symbol = true
    end
end

function PauseScene:update(delta)
    self.timer = self.timer + delta

    if self.resuming and self.timer >= 3 then
        self.game_state.set_scene(self.gameplay)
        return
    end

    if not self.resuming and self.timer >= 0.5 then
        self.timer = 0
        self.pause_symbol = not self.pause_symbol
        return
    end
end

function PauseScene:draw()
    if self.gameplay then
        self.gameplay:draw()
    end

    pico.push()
    pico.set.color.draw({r=0, g=0, b=0})
    pico.set.alpha(204)
    local dim_r = { '%', x=0.0, y=0.0, w=1.0, h=1.0, anchor='NW' }
    pico.output.draw.rect(dim_r)
    pico.pop()

    pico.set.color.draw("white")
    local txt_r = { '#', x=14.0, y=4.0, w=0.0, h=1.0, anchor='N' }

    if self.resuming then
        local fmt = tostring(3 - math.floor(self.timer))
        pico.output.draw.text(fmt, txt_r)
    elseif self.pause_symbol then
        pico.output.draw.text("||", txt_r)
    end
end

return PauseScene
