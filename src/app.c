#include "app.h"

int MAX_ITERATIONS = 100;

double n_x = -0.25f;
double n_y = 0.0f;

bool dragging = false;
int drag_x = 0;
int drag_y = 0;

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
	
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);

	return 0;
}

void app_run(App* app)
{
	bool quit = false;

	app_update(app);

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
	
	int x,y;
	SDL_GetMouseState(&x, &y);

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			app_quit(app);

			// this is just temporary cause idk what to do
			exit(0);
		}
		if (e.type == SDL_KEYDOWN) {
			app_update(app);
		}
		if (e.type == SDL_WINDOWEVENT){
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				app->screen_width = e.window.data1;
				app->screen_height = e.window.data2;
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			printf("MOUSE DOWN\n");
			
			app->is_dragging = true;

			app->drag_start_x = x;
			app->drag_start_y = y;

			printf("X: %d, Y: %d\n", x, y);

		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			printf("MOUSE UP\n");

			app->is_dragging = false;

			app->drag_end_x = x;
			app->drag_end_y = y;

			if (app->drag_start_x > app->drag_end_x) {
				int tmp = app->drag_start_x;
				app->drag_start_x = app->drag_end_x;
				app->drag_end_x = tmp;
			}

			if (app->drag_start_y > app->drag_end_y) {
				int tmp = app->drag_start_y;
				app->drag_start_y = app->drag_end_y;
				app->drag_end_y = tmp;
			}

			int end_point_x, end_point_y;
			if (abs(app->drag_start_x - app->drag_end_x) > abs(app->drag_start_y - app->drag_end_y)) {
				end_point_x = app->drag_start_x + abs(app->drag_start_x - app->drag_end_x);
				end_point_y = app->drag_start_y + abs(app->drag_start_x - app->drag_end_x);
			} else {
				end_point_x = app->drag_start_x + abs(app->drag_start_y - app->drag_end_y);
				end_point_y = app->drag_start_y + abs(app->drag_start_y - app->drag_end_y);
			}



			Coordinates new_start = screen_to_complex_no_zoom(app->drag_start_x, app->drag_start_y, app->screen_width, app->screen_height, app->start_point, app->end_point); 
			Coordinates new_end = screen_to_complex_no_zoom(end_point_x,end_point_y, app->screen_width, app->screen_height, app->start_point, app->end_point);
			

			app->start_point = new_start;
			
			app->end_point.x = new_end.x;
			app->end_point.y = new_end.y;


			MAX_ITERATIONS = (400 / sqrt(app->end_point.x - app->start_point.x));


			if (abs(MAX_ITERATIONS) > 1000000)
				MAX_ITERATIONS = 1000000;

			printf("MAX_ITERATIONS: %d\n", MAX_ITERATIONS);

			app_update(app);

		}

		if (app->is_dragging) {
			app->drag_end_x = x;
			app->drag_end_y = y;
		}
	}
}

void app_update(App* app)
{
	SDL_DestroyTexture(app->screen_buffer);
	app->screen_buffer = SDL_CreateTexture(
		app->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		app->screen_width,
		app->screen_height
	);


	Uint32* pixel_buffer = (Uint32*)malloc(app->screen_width * app->screen_height * sizeof(Uint32)); 

	for (int y = 0; y < app->screen_height; y++) {
		for (int x = 0; x < app->screen_width; x++) {
			PixelData pixel_data = fragment_compute(app, x, y);
			set_pixels(pixel_buffer, x, y, app->screen_width, app->screen_height, pixel_data.r, pixel_data.g, pixel_data.b, 255);
		}
	}
	update_texture(app->screen_buffer, pixel_buffer, app->screen_width, app->screen_height);
	free(pixel_buffer);
}

void app_draw(App* app)
{
	SDL_RenderClear(app->renderer);

	SDL_RenderCopy(app->renderer, app->screen_buffer, NULL, NULL);
	
	if (app->is_dragging) {
		SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
		SDL_Rect r = {app->drag_start_x, app->drag_start_y, app->drag_end_x - app->drag_start_x, app->drag_end_y - app->drag_start_y};
		SDL_RenderDrawRect(app->renderer, &r);
	}

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
		double  temp = x*x - y*y + cx;
		y = 2*x*y + cy;
		x = temp;

		iterations ++;
		if (iterations > MAX_ITERATIONS)
			return MAX_ITERATIONS;
	}
	return iterations;
}

PixelData fragment_compute(App* app, int input_cx, int input_cy)
{
	PixelData pixel_data =  {
		.r = 0,
		.b = 0,
		.g = 0
	};

	Coordinates coords = screen_to_complex_no_zoom(input_cx, input_cy, app->screen_width, app->screen_height, app->start_point, app->end_point); 

	double cx = coords.x;
	double cy = coords.y;

	int iterations = mandelbrot(cx, cy);
	
	mandelbrot_color(iterations, 100, &pixel_data.r, &pixel_data.g, &pixel_data.b);

	return pixel_data;
}
