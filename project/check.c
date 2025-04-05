#include <SDL.h>
#include <math.h>
#define bool int
#define false 0
#define true 1
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Function to draw a smooth line between two points
void drawSmoothLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        SDL_RenderDrawPoint(renderer, x1, y1);

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Function to draw a smooth point
void drawSmoothPoint(SDL_Renderer *renderer, int x, int y, double radius) {
    int r = (int)radius;
    for (int dx = -r; dx <= r; dx++) {
        for (int dy = -r; dy <= r; dy++) {
            double dist = sqrt(dx * dx + dy * dy);
            if (dist < radius) {
                Uint8 alpha = (Uint8)(255 * (1 - dist / radius));
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;
    int mouseX, mouseY;
    int prevMouseX, prevMouseY;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEMOTION) {
                prevMouseX = mouseX;
                prevMouseY = mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
            }
        }

        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            drawSmoothLine(renderer, prevMouseX, prevMouseY, mouseX, mouseY);
        } else {
            drawSmoothPoint(renderer, mouseX, mouseY, 5.0);
        }

        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
