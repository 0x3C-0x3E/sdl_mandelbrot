#include "app.h"

int MAX_ITERATIONS = 100;

double n_x = -0.25f;
double n_y = 0.0f;

int init_sdl() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Could not init SDL, Error: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}


int app_init(App* app) {
	if (init_sdl() != 0) {
		return 1;
	}
	
	app->window = SDL_CreateWindow(
		"Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		app->screen_width,
		app->screen_height,
		SDL_WINDOW_SHOWN
	);
	SDL_SetWindowResizable(app->window, SDL_TRUE);

	app->renderer = SDL_CreateRenderer(
		app->window, 
		-1,
		SDL_RENDERER_ACCELERATED
	);
	app->screen_buffer = SDL_CreateTexture(
		app->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		app->screen_width,
		app->screen_height
	);

	if (init_imgui(app->window, app->renderer) != 0) {
		return 1;
	}

	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);

	return 0;
}

void app_run(App* app)
{
	bool quit = false;

	while (!quit) {
		SDL_Delay(33);
		app_handle_events(app);
		//app_update(app);
		app_draw(app);
	}
}

void app_handle_events(App* app)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			app_quit(app);

			// this is just temporary cause idk what to do
			exit(0);
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_SPACE) {
				printf("COMPUTING!\n");
				app_update(app);
			}
		}
	}
}

void app_update(App* app)
{
	Uint32* pixel_buffer = (Uint32*)malloc(app->screen_width * app->screen_height * sizeof(Uint32)); 

	for (int y = 0; y < app->screen_height; y++) {
		for (int x = 0; x < app->screen_width; x++) {
			PixelData pixel_data = fragment_compute(app, x, y);
			set_pixels(pixel_buffer, x, y, app->screen_width, app->screen_height, pixel_data.r, pixel_data.g, pixel_data.b, 255);
		}
	}
	update_texture(app->screen_buffer, pixel_buffer, app->screen_width, app->screen_height);
}

void app_draw(App* app)
{
	SDL_RenderClear(app->renderer);
	ui_draw();
	SDL_RenderCopy(app->renderer, app->screen_buffer, NULL, NULL);

	SDL_RenderPresent(app->renderer);
}

void app_quit(App* app)
{
	SDL_DestroyTexture(app->screen_buffer);
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);

	SDL_Quit();
}

int mandelbrot(double cx, double cy)
{
	int iterations = 0;
	double x = 0.0f;
	double y = 0.0f;

	while(x*x + y*y <= 16.0f)
	{
		double temp = x*x - y*y + cx;
		y = 2*x*y + cy;
		x = temp;

		iterations ++;
		if (iterations > MAX_ITERATIONS)
			return 0;
	}
	return MAX_ITERATIONS;
}

PixelData fragment_compute(App* app, int input_cx, int input_cy)
{
	PixelData pixel_data =  {
		.r = 0,
		.b = 0,
		.g = 0
	};

	double cx = get_normalised_x((double) input_cx, (double) app->screen_height, n_x);
	double cy = get_normalised_y((double) input_cy, (double) app->screen_height, n_y);

	int iterations = mandelbrot(cx, cy);
	if (iterations == MAX_ITERATIONS) {
		pixel_data.r = 0;
		pixel_data.g = 0;
		pixel_data.b = 255;
	}

	return pixel_data;
}
