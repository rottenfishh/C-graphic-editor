#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include "paint.h"

void push(stack* stack, SDL_Point value)
{
    Node* newnode = (Node*) malloc(sizeof(Node));
    newnode->value = value;
    newnode->next = stack->top;
    stack->top = newnode;
}

SDL_Point pop(stack* stack)
{
    SDL_Point value = stack->top->value;
    Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return value;
}


SDL_Color GetPixelColor(SDL_Surface* surface, int x, int y) {
    Uint32 pixel = ((Uint32*)surface->pixels)[(y * surface->w) + x];
    SDL_Color color;
    SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
    color.a = 255; 
    return color;
}

void SetPixelColor(SDL_Surface* surface, int x, int y, SDL_Color color) {
    Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
    ((Uint32*)surface->pixels)[(y * surface->w) + x] = pixel;
}

int colorsMatch(SDL_Color a, SDL_Color b) {
    return (abs(a.r - b.r)<fill_diff && abs(a.g - b.g)<fill_diff && abs(a.b-b.b)<fill_diff && abs(a.a - b.a)<fill_diff);
}

void flood_fill(SDL_Renderer* renderer, int x, int y, SDL_Color init_color, SDL_Surface* surface, stack* top) {
    SDL_Color pixel_color = GetPixelColor(surface, x, y);
    if (!colorsMatch(pixel_color, init_color)) return;

    SDL_Point pix = {x,y};
    push(top, pix);

    while (top->top->next!=NULL) {
        SDL_Point p = pop(top);
        int px = p.x;
        int py = p.y;
        if (px < 0 || px >= surface->w || py < 100 || py >= surface->h)
            continue;
        pixel_color = GetPixelColor(surface, px, py);
        if (colorsMatch(pixel_color, init_color)) {
            SDL_RenderDrawPoint(renderer, px, py);
            SetPixelColor(surface, px, py, curr_color);
            push(top, (SDL_Point){px+1, py});
            push(top, (SDL_Point){px,py+1});
            push(top, (SDL_Point){px-1,py});
            push(top, (SDL_Point){px, py-1});
        }
    }
    if (top->top->next==NULL)
        return;
}
