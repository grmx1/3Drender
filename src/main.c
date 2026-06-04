#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "renderer.h"

const int RES[2] = {1000, 1000};

int main(){

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	int test = 0;
	int pollingRate = 500;
	double period = 1.00 / (double)pollingRate;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_CreateWindowAndRenderer(RES[0], RES[1], 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	struct object_3D main_obj = create_object_3D(0, 0, 0, 100, 100, 100);

	const Uint8* kbstate = SDL_GetKeyboardState(NULL);

	struct timespec start, end;
	struct timespec sleep_diff;

	double elapsed = 0;

	while(1){

		clock_gettime(CLOCK_MONOTONIC, &start);

		SDL_PumpEvents();

		if(kbstate[SDL_SCANCODE_ESCAPE]){

			return 0;
		}

		rotate_y(&main_obj, 0.01);
		project_object(&main_obj, RES[0], RES[1]);

		//clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
		render_object(renderer, &main_obj);

		SDL_RenderPresent(renderer);

		clock_gettime(CLOCK_MONOTONIC, &end);

		sleep_diff.tv_sec = end.tv_sec - start.tv_sec;
		sleep_diff.tv_nsec = end.tv_nsec - start.tv_nsec;

		elapsed = sleep_diff.tv_sec + sleep_diff.tv_nsec / 1e9;

		if(elapsed < period){

			sleep_diff.tv_nsec = period * 1e9 - sleep_diff.tv_nsec;

			nanosleep(&sleep_diff, NULL);
		}

	}
}
