#include <SDL.h>
#include <stdio.h>
#include "app.h"

int main(int argc, char* argv[]) {
	
	App app = {
		.screen_width = 800,
		.screen_height = 800,
		.zoom = 5.0f,
		.offset_x = -1.5f,
		.offset_y = 0.0f,
	};

	app_init(&app);
	app_run(&app);

	return 0;
}

 
