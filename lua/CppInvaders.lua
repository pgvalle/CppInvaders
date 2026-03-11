local assets = require "assets"
local SplashScene = require "scenes.Splash"

local game = {
    should_quit = false,
    score = 0,
    hi_score = 0,
    credits = 0,
    keys = {},
    current_scene = nil,
    next_scene = nil
}

function game.set_scene(new_scene)
    game.next_scene = new_scene
end

local f = io.open("scoreboard", "r")
if f then
    game.hi_score = tonumber(f:read("*a")) or 0
    f:close()
end

function game.save_hi_score()
    local fw = io.open("scoreboard", "w")
    if fw then
        fw:write(tostring(game.hi_score))
        fw:close()
    end
end

function game.add_to_score(val)
    game.score = game.score + val
    if game.score > game.hi_score then
        game.hi_score = game.score
    end
end

local function process_event(event)
    if event.tag == 'quit' then
        game.should_quit = true
    else
        if event.tag == 'key.dn' then
            game.keys[event.key] = true
        elseif event.tag == 'key.up' then
            game.keys[event.key] = false
        end

        if game.current_scene and game.current_scene.process_event then
            game.current_scene:process_event(event)
        end
    end
end

function game.draw_indicators()
    pico.set.color.draw("white")

    local tl_corner = { '%', x=0.0, y=0.0, w=0.0, h=0.0, anchor='NW' }
    local score_lbl_r = { '#', x=2.0, y=2.0, w=0.0, h=1.0, anchor='NW', up=tl_corner }
    pico.output.draw.text("YOUR SCORE", score_lbl_r)

    local score_buf = string.format("%06d", game.score)
    local score_val_r = { '#', x=4.0, y=4.0, w=0.0, h=1.0, anchor='NW', up=score_lbl_r }
    pico.output.draw.text(score_buf, score_val_r)

    local tr_corner = { '%', x=1.0, y=0.0, w=0.0, h=0.0, anchor='NE' }
    local hi_lbl_r = { '#', x=-1.0, y=2.0, w=0.0, h=1.0, anchor='NE', up=tr_corner }
    pico.output.draw.text("HIGH-SCORE", hi_lbl_r)

    local hi_buf = string.format("%06d", game.hi_score)
    local hi_val_r = { '#', x=-3.0, y=4.0, w=0.0, h=1.0, anchor='NE', up=hi_lbl_r }
    pico.output.draw.text(hi_buf, hi_val_r)

    local br_corner = { '%', x=1.0, y=1.0, w=0.0, h=0.0, anchor='SE' }
    local credit_buf = string.format("CREDIT %02d", game.credits)
    local credit_r = { '#', x=-1.0, y=-1.0, w=0.0, h=1.0, anchor='SE', up=br_corner }
    pico.output.draw.text(credit_buf, credit_r)
end

function game.draw()
    pico.output.clear()
    game.draw_indicators()
    if game.current_scene and game.current_scene.draw then
        game.current_scene:draw()
    end
    pico.output.present()
end

local FPS = 60
local FRAMETIME = 1000 / FPS

function game.main()
    pico.init(true)
    pico.set.window({ title = "CppInvaders" })
    pico.set.window({ dim = { '!', w=448, h=512 } })
    pico.set.view({ dim = { '!', w=224, h=256 }, tile = { w=8, h=8 } })
    pico.set.expert(true)
    pico.set.font(assets.FONT)

    math.randomseed(os.time())
    game.set_scene(SplashScene:new(game))

    local delta = 0
    while not game.should_quit do
        if game.next_scene then
            game.current_scene = game.next_scene
            game.next_scene = nil
        end

        local start_ticks = pico.get.ticks()
        local passed = 0
        while passed < FRAMETIME do
            local timeout = math.max(0, math.floor(FRAMETIME - passed))
            local event = pico.input.event(timeout)
            if event then
                process_event(event)
            end
            passed = pico.get.ticks() - start_ticks
        end
        game.draw()
        
        local dt = delta * 0.001
        if dt > 0.1 then dt = 0.1 end
        if game.current_scene and game.current_scene.update then
            game.current_scene:update(dt)
        end
        delta = pico.get.ticks() - start_ticks
    end

    game.save_hi_score()
    pico.init(false)
end

return game
