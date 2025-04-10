#pragma once
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "ui.h"

typedef struct App {
	SDL_Window * window;
	SDL_Renderer * renderer;
	int screen_width;
	int screen_height;
	SDL_Texture * screen_buffer;
} App;

extern int MAX_ITERATIONS;

extern double n_x;
extern double n_y;

int init_sdl();

int app_init(App* app);

void app_run(App* app);

void app_handle_events(App* app);

void app_update(App* app);

void app_draw(App* app);

void app_quit(App* app);

int mandelbrot(double cx, double cy); // returns the iterations

PixelData fragment_compute(App* app, int input_cx, int input_cy);

