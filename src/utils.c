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
