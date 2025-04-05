typedef struct Node_s Node;
typedef struct stack stack;
void push(stack* stack, SDL_Point value);
SDL_Point pop(stack* stack);
SDL_Color GetPixelColor(SDL_Surface* surface, int x, int y);
void SetPixelColor(SDL_Surface* surface, int x, int y, SDL_Color color);
int colorsMatch(SDL_Color a, SDL_Color b);
void flood_fill(SDL_Renderer* renderer, int x, int y, SDL_Color init_color, SDL_Surface* surface);