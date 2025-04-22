#include <SDL.h>
#include <stdio.h>
#include "app.h"

int main(int argc, char* argv[]) {
	
	App app = {
		.screen_width = 800,
		.screen_height = 800,
		.start_point = {
			.x = -2.0f,
			.y = -2.0f,
		},
		.end_point =  {
			.x = 2.0f,
			.y = 2.0f,
		},
		.is_dragging = false,
	};

	app_init(&app);
	app_run(&app);

	return 0;
}

 
