#include <SDL.h>
#include <SDL_image.h>
extern int BrushSize;
extern int fill_diff;
extern int pr_x, pr_y, curr_x, curr_y;
extern double Kernel[20][20];
extern SDL_Color curr_color;
typedef struct Node_s
{
    SDL_Point value;
    struct Node_s* next;
}Node;

typedef struct stack_s
{
    Node* top;
} stack;

typedef struct mode {
    int filling;
    int color_picking;
    int blur;
} curr_mode;
typedef struct brush{
    int circle;
    int square;
    int diamond;
    int circle_outline;
    int rect_outline;
}brush;

void push(stack* stack, SDL_Point value);
SDL_Point pop(stack* stack);
SDL_Color GetPixelColor(SDL_Surface* surface, int x, int y);
void SetPixelColor(SDL_Surface* surface, int x, int y, SDL_Color color);
int colorsMatch(SDL_Color a, SDL_Color b);
void flood_fill(SDL_Renderer* renderer, int x, int y, SDL_Color init_color, SDL_Surface* surface, stack* top);
void createKernel(int radius);
void apply_Gauss(SDL_Surface* surface_orig, SDL_Surface* surface_copy, int x, int y, int radius);
void drawCircle(SDL_Renderer* renderer, int x, int y);
void drawSquareBrush(SDL_Renderer* renderer, int x, int y);
void drawCircleBrush(SDL_Renderer* renderer, int x, int y);
void drawDiamondBrush(SDL_Renderer* renderer, int x, int y);
void drawCircleOutline(SDL_Renderer* renderer, int centerX, int centerY);
void drawLine(SDL_Renderer* renderer, int x, int y, int prevx, int prevy);
void drawDiamondLine(SDL_Renderer* renderer, int x, int y, int prevx, int prevy);
void drawSquareLine(SDL_Renderer* renderer, int x, int y, int prevx, int prevy);
void draw_rect_outline(SDL_Renderer* renderer, int x, int y);
void drawincompleteCircle(SDL_Renderer* renderer, int x, int y);
void setBrushmode(brush* mode, int c, int s, int d, int co, int r);
void setRightmode(curr_mode* mode, int f, int c, int b);
void curr_tool(SDL_Renderer* renderer, int x, int y);