#include <string>
#include <iostream>
#include <SDL.h>
#undef main

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Pix", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_RenderSetLogicalSize(renderer, 512, 512);

	SDL_SetRenderDrawColor(renderer, 10, 130, 127, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
	SDL_RenderDrawPoint(renderer, 256, 128);

	SDL_RenderPresent(renderer);


	SDL_Delay(3000);
	SDL_PollEvent(0);

	std::string temp;
	std::cin >> temp;
	return 0;
}