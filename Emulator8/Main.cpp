#include <SDL3/SDL.h>

#include <iostream>
#include "Chip8.h"
bool InitVideo(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("Title", 640, 320, 0);
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!window || !renderer) {
		std::cout << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}
int main() {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	Chip8 chip8;
	chip8.loadROM("tetris.rom");	



	SDL_FRect pixel = { 0,0, 10, 10 };
	bool running = InitVideo(window, renderer);
	while (running)
	{

	
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
			

		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		for (int row = 0; row < 32; row++)
		{
			for (int col = 0; col < 64; col++)
			{
				pixel.x = 10 * col;
				pixel.y = 10 * row;
				if (chip8.display[row][col] == 1)
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				else
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

				SDL_RenderFillRect(renderer, &pixel);

				
			}
		}
		SDL_RenderPresent(renderer);
	}
    return 0;
}