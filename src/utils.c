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

Coordinates screen_to_complex_no_zoom(int x, int y, int screen_width, int screen_height, Coordinates start_point, Coordinates end_point)
{
	double ratio = (double) x / (double) screen_width;
	double dx = end_point.x - start_point.x;
	Coordinates return_value = {
		.x = start_point.x + fabs((ratio) * (dx)),
		.y = start_point.y + ((double)y / screen_width) * (end_point.y - start_point.y),
	};
	
	return return_value;
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

Color lerp(Color c1, Color c2, float t) {
    Color result;
    result.r = (uint8_t)((1.0f - t) * c1.r + t * c2.r);
    result.g = (uint8_t)((1.0f - t) * c1.g + t * c2.g);
    result.b = (uint8_t)((1.0f - t) * c1.b + t * c2.b);
    return result;
}

void mandelbrot_color(int iterations, int MAX_ITERATIONS, uint8_t *r, uint8_t *g, uint8_t *b) {
	float position = (float)iterations / MAX_ITERATIONS;
    // Define the gradient
    static const ColorStop gradient[] = {
        {0.0f,     0,   7, 100},
        {0.16f,   32, 107, 203},
        {0.42f,  237, 255, 255},
        {0.6425f,255, 170,   0},
        {0.8575f,  0,   2,   0}
    };

    const int num_stops = sizeof(gradient) / sizeof(gradient[0]);

    // Clamp position to [0, 1]
    if (position <= gradient[0].position) {
        *r = gradient[0].r;
        *g = gradient[0].g;
        *b = gradient[0].b;
        return;
    }
    if (position >= gradient[num_stops - 1].position) {
        *r = gradient[num_stops - 1].r;
        *g = gradient[num_stops - 1].g;
        *b = gradient[num_stops - 1].b;
        return;
    }

    // Find the interval
    for (int i = 0; i < num_stops - 1; ++i) {
        if (position >= gradient[i].position && position <= gradient[i + 1].position) {
            float local_t = (position - gradient[i].position) /
                            (gradient[i + 1].position - gradient[i].position);
            Color a = {gradient[i].r, gradient[i].g, gradient[i].b};
            Color b1 = {gradient[i + 1].r, gradient[i + 1].g, gradient[i + 1].b};
            Color c = lerp(a, b1, local_t);
            *r = c.r;
            *g = c.g;
            *b = c.b;
            return;
        }
    }
}
