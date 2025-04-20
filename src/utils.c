#include "utils.h"

void update_texture(SDL_Texture* texture, Uint32* pixelBuffer, int width, int height) {
    // Update the texture with the entire pixel buffer
    SDL_UpdateTexture(texture, NULL, pixelBuffer, width * sizeof(Uint32));
}

void set_pixels(Uint32* pixelBuffer, int x, int y, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    Uint32 color = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), r, g, b, a);
    
    pixelBuffer[y * width + x] = color;

    SDL_FreeFormat(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888));
}


double get_normalised_x(double x, double screen_width, double n_x)
{

	return ((x - screen_width / 2) + n_x * screen_width) / (screen_width / 2);
}

double get_normalised_y(double y, double screen_height, double n_y)
{
	return ((screen_height / 2 - y) + n_y * screen_height) / (screen_height / 2);
}

Coordinates screen_to_complex(int x, int y, int screen_width, int screen_height, double zoom, double offset_x, double offset_y)
{
	double scale = 4 / zoom;

	Coordinates coords = {
		.x = (x - screen_width / 2) * (scale / screen_width) + offset_x,
		.y = (y - screen_height / 2) * (scale / screen_width) + offset_y
	};

	return coords;
}


int clamp(double x)
{
	if (x < 0)
		return 0;
	if (x > 255)
		return 255;

	return x;
}

void get_color(int iterations, int MAX_ITERATIONS, Uint8 * r, Uint8 * g, Uint8 * b)
{
	if (iterations == MAX_ITERATIONS) {
        *r = *g = *b = 0; 
        return;
    }

    double t = (double)iterations / MAX_ITERATIONS;

    // Smooth gradient (e.g., blue to red via purple)
    *r = clamp(9 * (1 - t) * t * t * t * 255);
    *g = clamp(15 * (1 - t) * (1 - t) * t * t * 255);
    *b = clamp(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}
