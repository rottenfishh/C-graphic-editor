#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include "paint.h"
void createKernel(int radius){
    double sigma = (double)radius/2.0 >1 ? (double)radius/2.0: 1;
    int kernelwidth = (radius * 2)+1;
    double sum = 0;
    for (int x=-radius; x<radius; x++){
        for (int y=-radius; y<radius; y++){
            double exponent = -(x * x + y * y) / (2 * sigma * sigma);
            double kernelvalue = exp(exponent) / (2*M_PI*sigma*sigma);
            Kernel[x + radius][y + radius] = kernelvalue;
            sum+=kernelvalue;
        }
    }
    for (int x=0; x<kernelwidth-1; x++){
        for (int y=0; y<kernelwidth-1; y++){
            Kernel[x][y]/=sum;
        }
    }
}
void apply_Gauss(SDL_Surface* surface_orig, SDL_Surface* surface_copy, int x, int y, int radius) {
    int* orig_pixels = surface_orig->pixels;
    int* copy_pixels = surface_copy->pixels;
    int width = surface_orig->w;
    int height = surface_orig->h;
    int start_x = x - BrushSize < 0 ? 0 : x - BrushSize;
    int end_x = x + BrushSize >= width ? width - 1 : x + BrushSize;
    int start_y = y - BrushSize < 0 ? 0 : y - BrushSize;
    int end_y = y + BrushSize >= height ? height - 1 : y + BrushSize;
    int kernelwidth = (radius * 2) + 1;
    for (int i = start_x; i < end_x; i++) {
        for (int j = start_y; j < end_y; j++) {
            double red = 0, green = 0, blue = 0;
            for (int kernelX = -radius; kernelX < radius; kernelX++) {
                for (int kernelY = -radius; kernelY < radius; kernelY++) {
                    int pixelX = i + kernelX;
                    int pixelY = j + kernelY;

                    if (pixelX >= 0 && pixelX < width && pixelY >= 0 && pixelY < height) {
                        double kernelvalue = Kernel[kernelX + radius][kernelY + radius];
                        Uint32 pixel = orig_pixels[pixelY * width + pixelX];
                        SDL_Color color;
                        SDL_GetRGB(pixel, surface_orig->format, &color.r, &color.g, &color.b);
                        red += color.r * kernelvalue;
                        green += color.g * kernelvalue;
                        blue += color.b * kernelvalue;
                    }
                }
            }
            red = (red < 0) ? 0 : (red > 255) ? 255 : red;
            green = (green < 0) ? 0 : (green > 255) ? 255 : green;
            blue = (blue < 0) ? 0 : (blue > 255) ? 255 : blue;
            SDL_Color blurred_color = {(Uint8)red, (Uint8)green, (Uint8)blue, 255};
            SetPixelColor(surface_copy, i, j, blurred_color);
        }
    }
}