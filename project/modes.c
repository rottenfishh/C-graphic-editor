#include "paint.h"
#include <SDL.h>
#include <SDL_image.h>
void setBrushmode(brush* mode, int c, int s, int d, int co, int r){
    (*mode).circle = c;
    (*mode).diamond = s;
    (*mode).square = d;
    (*mode).circle_outline=co;
    (*mode).rect_outline =r;
}

void setRightmode(curr_mode* mode, int f, int c, int b){
    (*mode).filling = f;
    (*mode).color_picking = c;
    (*mode).blur = b;
}

void curr_tool(SDL_Renderer* renderer, int x, int y){
    pr_x = curr_x;
    pr_y = curr_y;
    curr_x = x;
    curr_y = y;
    SDL_Rect prev_rect = {pr_x - (pr_x%50)+1, pr_y-(pr_y%50)+1,50,50};
    SDL_SetRenderDrawColor(renderer, 193, 189, 189, 255);
    SDL_RenderDrawRect(renderer, &prev_rect);
    SDL_Rect curr_rect = {x - (x%50) +1, y - (y%50) +1, 50,50};
    SDL_SetRenderDrawColor(renderer,0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &curr_rect);
    SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
}