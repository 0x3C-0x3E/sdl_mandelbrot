#pragma once
#include <stdio.h>
#include <stdint.h>
#include <SDL.h>
#include <math.h>

void update_texture(SDL_Texture* texture, Uint32* pixelBuffer, int width, int height);

void set_pixels(Uint32* pixelBuffer, int x, int y, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


typedef struct PixelData {
	Uint8 r;
	Uint8 b;
	Uint8 g;

} PixelData;


typedef struct Coordinates {
	double x;
	double y;
} Coordinates;

typedef struct ColorStop{
    float position;
    uint8_t r, g, b;
} ColorStop;

typedef struct Color{
    uint8_t r, g, b;
} Color;


Coordinates screen_to_complex(int x, int y, int screen_width, int screen_height, double zoom, double offset_x, double offset_y);

Coordinates screen_to_complex_no_zoom(int x, int y, int screen_width, int screen_height, Coordinates start_point, Coordinates end_point);

double get_normalised_x(double x, double screen_width, double n_x);

double get_normalised_y(double y, double screen_height, double n_y);

int clamp(double x);

void get_color(int iterations, int MAX_ITERATIONS, Uint8 * r, Uint8 * g, Uint8 * b);

Color lerp(Color c1, Color c2, float t);

void mandelbrot_color(int iterations, int MAX_ITERATIONS, uint8_t *r, uint8_t *g, uint8_t *b);

