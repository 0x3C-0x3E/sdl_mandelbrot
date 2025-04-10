#pragma once
#include <SDL.h>

void update_texture(SDL_Texture* texture, Uint32* pixelBuffer, int width, int height);

void set_pixels(Uint32* pixelBuffer, int x, int y, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


typedef struct PixelData {
	Uint8 r;
	Uint8 b;
	Uint8 g;

} PixelData;


double get_normalised_x(double x, double screen_width, double n_x);

double get_normalised_y(double y, double screen_height, double n_y);
