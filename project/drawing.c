#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include "paint.h"
void drawCircle(SDL_Renderer* renderer, int x, int y) {
    for (int i = 0; i <= BrushSize * 2; i++) {
        for (int j = 0; j <= BrushSize * 2; j++) {
            int dx = BrushSize - i; 
            int dy = BrushSize - j;
            if (dx * dx + dy * dy <= BrushSize * BrushSize) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void drawSquareBrush(SDL_Renderer* renderer, int x, int y) {
    for (int i = -BrushSize; i <= BrushSize; i++) {
        for (int j = -BrushSize; j <= BrushSize; j++) {
            SDL_RenderDrawPoint(renderer, x + i, y + j);
        }
    }
}

void drawCircleBrush(SDL_Renderer* renderer, int x, int y) {
    for (int i = -BrushSize; i <= BrushSize; i++) {
        for (int j = -BrushSize; j <= BrushSize; j++) {
            if (i * i + j * j <= BrushSize * BrushSize) {
                SDL_RenderDrawPoint(renderer, x + i, y + j);
            }
        }
    }
}

void drawDiamondBrush(SDL_Renderer* renderer, int x, int y) {
    for (int i = -BrushSize; i <= BrushSize; i++) {
        for (int j = -BrushSize; j <= BrushSize; j++) {
            if (abs(i) + abs(j) <= BrushSize) {
                SDL_RenderDrawPoint(renderer, x + i, y + j);
            }
        }
    }
}

void drawCircleOutline(SDL_Renderer* renderer, int centerX, int centerY) {
    int x = 0;          
    int y = BrushSize;
    int d = 3 - 2 * BrushSize; 

    while (y >= x){ 
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

        x++; 

        if (d > 0) {
            y--; 
            d = d + 4 * (x - y) + 10; 
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void drawLine(SDL_Renderer* renderer, int x, int y, int prevx, int prevy) {
    for (int i = 0; i <= BrushSize * 2; i++) {
        for (int j = 0; j <= BrushSize * 2; j++) {
            int dx = BrushSize - i;
            int dy = BrushSize - j; 
            if (dx * dx + dy * dy <= BrushSize * BrushSize) {
                SDL_RenderDrawLine(renderer, x + dx, y + dy, prevx + dx, prevy + dy);
            }
        }
    }
}

void drawDiamondLine(SDL_Renderer* renderer, int x, int y, int prevx, int prevy){
    for (int i = -BrushSize; i <= BrushSize; i++) {
        for (int j = -BrushSize; j <= BrushSize; j++) {
            if (abs(i) + abs(j) <= BrushSize) {
                SDL_RenderDrawLine(renderer, x + i, y + j, prevx+i, prevy+j);
            }
        }
    }
}
void drawSquareLine(SDL_Renderer* renderer, int x, int y, int prevx, int prevy){
    for (int i = -BrushSize; i <= BrushSize; i++) {
        for (int j = -BrushSize; j <= BrushSize; j++) {
            SDL_RenderDrawLine(renderer, x + i, y + j, prevx+i, prevy+j);
        }
    }
}
void draw_rect_outline(SDL_Renderer* renderer, int x, int y){
    SDL_RenderDrawLine(renderer, x - BrushSize, y- BrushSize, x+BrushSize, y - BrushSize);
    SDL_RenderDrawLine(renderer, x + BrushSize, y - BrushSize, x+BrushSize, y + BrushSize);
    SDL_RenderDrawLine(renderer, x - BrushSize, y + BrushSize, x+BrushSize, y + BrushSize);
    SDL_RenderDrawLine(renderer, x - BrushSize, y + BrushSize, x-BrushSize, y - BrushSize);
}

void drawincompleteCircle(SDL_Renderer* renderer, int x, int y) {
    int count=0;
    for (int i = 0; i <= BrushSize * 2; i++) {
        count++;
        if (count%3==0)
            continue;
        for (int j = 0; j <= BrushSize * 2; j++) {
            int dx = BrushSize - i; 
            int dy = BrushSize - j; 
            if (dx * dx + dy * dy <= BrushSize * BrushSize) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}
