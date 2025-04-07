#pragma once
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

typedef struct App {
	SDL_Window * window;
	SDL_Renderer * renderer;
	int screen_width;
	int screen_height;
	SDL_Texture * screen_buffer;
} App;

const int MAX_ITERATIONS = 2;

int init_sdl();

int app_init(App* app);

void app_run(App* app);

void app_handle_events(App* app);

void app_update(App* app);

void app_draw(App* app);

void app_quit(App* app);

PixelData mandelbrot(App* app, int input_cx, int input_cy);
