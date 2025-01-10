#include "global.h"

int credits, score, hi_score;
Screen *current_screen, *next_screen;

void draw_ui_elements() {
    pico_output_draw_text({ 8, 8 }, "YOUR SCORE      HIGH-SCORE");

    static char text[32];
    
    sprintf(text, "%06d          %06d", score, hi_score);
    pico_output_draw_text({ 24, 24 }, text);

    sprintf(text, "CREDIT %02d", credits);
    pico_output_draw_text({ 144, 240 }, text);
}