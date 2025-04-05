#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include "paint.h"
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 1000
#define PALETTE_SIZE 24
#define MAX_BRUSH_SIZE 50
#define MAX_HEIGHT 900
#define MAX_WIDTH 1500
int BrushSize = 1;
int fill_diff = 1;
int pr_x = 750, pr_y=0, curr_x=750, curr_y=0;
double Kernel[20][20];
int canvas_posx, canvas_posy, rx,ry;
SDL_Color curr_color = {0, 0, 0, 255};
SDL_Color palette[PALETTE_SIZE] = {
    {0, 0, 0, 255}, // black 1
    {128, 128, 128, 255}, // grey 7
    {255, 0, 0, 255}, // red 2
    {0, 255, 0, 255}, // light green 3
    {0, 128, 0, 255}, // dark green 4
    {0, 0, 255, 255}, // blue 5
    {0, 255, 255, 255}, // cyan 6
    {255, 255, 0, 255}, // yellow 8
    {165, 42, 42, 255}, // brown 9
    {255, 165, 0, 255}, // orange 10
    {128, 0, 128, 255}, // purple 11
    {255,105,180,255}, //pink 12
    {255, 255, 255, 255}, // white 0
    {220,220,220,255}, //light grey
    {250, 128, 114, 255}, //light red
    {118, 255, 122, 255}, //green
    {3,192,60,255}, //green
    {70,130,180,255}, //blue
    {185,242,255,255}, //cyan
    {253,253,150,255}, //yellow
    {193,154,107,255}, // light brown
    {255,218,185,255}, // peach
    {221,160,221,255}, //light purple
    {255, 192, 203, 255} // pink 12
};

int main(int argc, char* argv[]) {
    int IsRunning = 1;
    int x, y;
    int prevx = -1, prevy = -1;
    SDL_Event event;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* windowSurface = NULL;
    SDL_Surface* imageSurface = NULL; 
    SDL_Surface* image = NULL;
    SDL_Texture* texture = NULL;
    SDL_Surface* canvassurface = NULL;
    SDL_Surface* start_image = NULL;
    SDL_Texture* canvastexture = NULL;
    int textureWidth, textureHeight;
    char open_name[100];
    char save_name[100];
    char flag;
    stack* top;
    top = (stack*) malloc(sizeof(stack));
    top->top = NULL;
    curr_mode mode = {1, 0,0};
    brush brush = {1,0,0,0,0};
    createKernel(5);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    start_image = IMG_Load("canvas2.png");
    printf("do you want to load image? y/n\n");
    scanf("%s", &flag);
    if (flag=='y'){
        printf("name of the image you want to load:\n");
        scanf("%s", open_name);
        image = IMG_Load(open_name);
        if (!image) {
            printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        textureWidth = image->w;
        textureHeight = image->h;
    }
    else{
        printf("Choose canvas width:\n");
        scanf("%d", &textureWidth);
        printf("Choose canvas height:\n");
        scanf("%d", &textureHeight);
    }
    textureWidth = (textureWidth>MAX_WIDTH? MAX_WIDTH : textureWidth);
    textureHeight = (textureHeight>MAX_HEIGHT? MAX_HEIGHT : textureHeight);
    canvas_posy = 100;
    canvas_posx = 0;
    rx = canvas_posx+textureWidth;
    ry = canvas_posy+textureHeight;
    SDL_Rect canvas = {canvas_posx,canvas_posy, textureWidth, textureHeight};
    canvassurface = SDL_CreateRGBSurface(0, textureWidth, textureHeight, 32,0,0,0,0);
    imageSurface = image;
    window = SDL_CreateWindow("Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    windowSurface = SDL_GetWindowSurface(window);
    texture = SDL_CreateTextureFromSurface(renderer, start_image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    canvastexture = SDL_CreateTextureFromSurface(renderer, canvassurface);
    SDL_UpdateWindowSurface(window);
    if (flag=='y'){
        SDL_BlitScaled(image, &canvas, start_image, &canvas);
        texture = SDL_CreateTextureFromSurface(renderer, start_image);
        SDL_RenderCopy(renderer, texture,&canvas, &canvas);
    }
    else{
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &canvas);
    }
    SDL_RenderPresent(renderer);
    while (IsRunning) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                IsRunning = 0;
            if (event.type == SDL_KEYDOWN) {
                if ((event.key.keysym.sym == SDLK_EQUALS || event.key.keysym.sym == SDLK_PLUS) && BrushSize<=MAX_BRUSH_SIZE) {
                    BrushSize++;
                    drawCircle(renderer, 680, 50);
                    SDL_RenderPresent(renderer);
                } else if (event.key.keysym.sym == SDLK_MINUS && BrushSize > 0) { //change brush size
                    BrushSize--;
                    SDL_SetRenderDrawColor(renderer, 193, 189, 189, 255);
                    int temp = BrushSize;
                    BrushSize = MAX_BRUSH_SIZE+1;
                    drawCircle(renderer, 680, 50);
                    BrushSize=temp;
                    SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
                    drawCircle(renderer, 680, 50);
                    SDL_RenderPresent(renderer);
                }
                if (event.key.keysym.mod == KMOD_LCTRL && event.key.keysym.sym == SDLK_x) { //clear screen
                    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                    SDL_RenderFillRect(renderer, &canvas);
                    SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
                    SDL_RenderPresent(renderer);
                }
                if (event.key.keysym.mod == KMOD_LCTRL && event.key.keysym.sym == SDLK_s) { //save image
                    SDL_RenderReadPixels(renderer, &canvas, 0, canvassurface->pixels, canvassurface->pitch);
                    printf("name your drawing:\n");
                    scanf("%s", save_name);
                    IMG_SavePNG(canvassurface, save_name);
                    SDL_RenderPresent(renderer);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                x = event.button.x;
                y = event.button.y;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (y < 50 && x <= 600) {
                        int colorIndex = x / 50;
                        curr_color = palette[colorIndex];
                        SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
                        drawCircle(renderer, 680, 50);
                        SDL_RenderPresent(renderer); //change color
                    }
                    else if (y<100 && y>50 && x>1050 && x<1100){
                        if (fill_diff>1)
                            fill_diff--;
                    }
                    else if (y<100 && y>50 && x>1100 && x<1150){
                        if (fill_diff<25)
                            fill_diff++;
                    }
                    else if (y<100 && x<=600){
                        int colorIndex = x / 50 +12;
                        curr_color = palette[colorIndex];
                        SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
                        drawCircle(renderer, 680, 50);
                        SDL_RenderPresent(renderer); //change color
                    }
                    else if (y<50){
                        if (x>750 && x<800){
                            curr_tool(renderer,x, y);
                            SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);//brush
                            drawCircle(renderer, 680, 50);
                            SDL_RenderPresent(renderer);
                        }
                        else if (x>800 && x<850){
                            curr_tool(renderer,x, y);
                            setBrushmode(&brush, 1,0,0,0,0);
                            SDL_SetRenderDrawColor(renderer, 255,255,255,255); //eraser
                            drawCircle(renderer, 680, 50);
                            SDL_RenderPresent(renderer); 
                        }
                        else if (x>850 && x<900){ // filling
                            curr_tool(renderer,x, y);
                            setRightmode(&mode, 1, 0,0);
                        }
                        else if (x>900&& x<950){ // pipetka
                            curr_tool(renderer,x, y);
                            setRightmode(&mode, 0, 1,0);
                        }
                        else if (x>950&& x<1000){ // blur
                            curr_tool(renderer,x, y);
                            setRightmode(&mode, 0, 0,1);
                        }
                    }
                    else if (y<100 && y>50){
                        if (x>750 && x<800){
                            curr_tool(renderer,x, y);
                            setBrushmode(&brush, 1,0,0,0,0);
                        }
                        else if (x>800 && x<850){
                            curr_tool(renderer,x, y);
                            setBrushmode(&brush, 0,1,0,0,0);
                        }
                        else if (x>850 && x<900){
                            curr_tool(renderer,x, y);
                            setBrushmode(&brush, 0,0,1,0,0);
                        }
                        else if (x>900 && x<950){
                            curr_tool(renderer,x, y);
                            setBrushmode(&brush, 0,0,0,1,0);
                        }
                        else if (x>950 && x<1000){
                            curr_tool(renderer,x, y);
                            setBrushmode(&brush, 0,0,0,0,1);
                        }
                    }
                    while (event.type != SDL_MOUSEBUTTONUP) { //draw
                        prevx = x;
                        prevy = y;
                        SDL_PollEvent(&event);
                        if (y-BrushSize>100 && y-BrushSize>canvas_posy && y+BrushSize<ry && x-BrushSize >canvas_posx && x+BrushSize<rx)
                            if (brush.circle)
                                drawCircle(renderer,x,y);
                            else if (brush.diamond)
                                drawDiamondBrush(renderer, x, y);
                            else if (brush.square)
                                drawSquareBrush(renderer, x, y);
                            else if (brush.circle_outline)
                                drawCircleOutline(renderer,x,y);
                            else if (brush.rect_outline)
                                draw_rect_outline(renderer,x,y);
                        if (event.type == SDL_MOUSEMOTION) {
                            x = event.motion.x;
                            y = event.motion.y;
                            if ((x != prevx || y != prevy) && prevx != -1 && prevy != -1 && y-BrushSize > 100 &&
                             y-BrushSize>=canvas_posy && y+BrushSize<=ry && x -BrushSize>=canvas_posx && x+BrushSize<=rx
                             &&prevy-BrushSize>=canvas_posy && prevy+BrushSize<=ry && prevx -BrushSize>=canvas_posx && prevx+BrushSize<=rx) {
                                if (brush.circle)
                                    drawLine(renderer, x, y, prevx, prevy);
                                else if (brush.diamond)
                                    drawDiamondLine(renderer, x, y, prevx, prevy);
                                else if (brush.square)
                                    drawSquareLine(renderer, x, y, prevx, prevy);
                            }
                        }
                        SDL_RenderPresent(renderer);
                    }
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (y>100 && y>canvas_posy && y<ry && x >canvas_posx && x<rx){
                        if (mode.filling){
                            SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, windowSurface->pixels, windowSurface->pitch);
                            SDL_Color init_color = GetPixelColor(windowSurface, x, y);
                            flood_fill(renderer, x, y, init_color, windowSurface, top);
                            SDL_RenderPresent(renderer);
                        }
                        else if (mode.color_picking){
                            SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, windowSurface->pixels, windowSurface->pitch);
                            curr_color = GetPixelColor(windowSurface, x, y);
                            SDL_SetRenderDrawColor(renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
                            drawCircle(renderer, 680, 50);
                            SDL_RenderPresent(renderer); 
                        }
                        else if (mode.blur){
                            SDL_Surface* surface1 = SDL_CreateRGBSurface(0, textureWidth, textureHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
                            SDL_Surface* surface2 = SDL_CreateRGBSurface(0, textureWidth, textureHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
                            SDL_RenderReadPixels(renderer,&canvas, 0, surface1->pixels, surface1->pitch);
                            SDL_RenderReadPixels(renderer,&canvas, 0, surface2->pixels, surface2->pitch);
                            apply_Gauss(surface1, surface2, x,y,5);
                            canvastexture = SDL_CreateTextureFromSurface(renderer, surface2);
                            SDL_RenderCopy(renderer, canvastexture, &canvas, &canvas);
                            SDL_RenderPresent(renderer);
                        }
                    }
                }
            }
        }
    }
    SDL_FreeSurface(windowSurface);
    SDL_FreeSurface(image);
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(canvassurface);
    SDL_FreeSurface(start_image);
    SDL_DestroyTexture(canvastexture);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    free(top);
    SDL_Quit();
    return 0;
}