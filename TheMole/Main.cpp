#include <iostream>
#include <unordered_map>
#include <string>
#include <SDL2pp\SDL2pp.hh>
#include <SDL2\SDL_main.h>
#include <memory>
#include "GameManager.h"
#include "MenuScreen.h"
#include "Input.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;

int main(int argc, char** argv) {
	try {
		using namespace SDL2pp;
		uint32_t subsystems = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER;
		SDL sdl(subsystems);
		SDLTTF sdl_ttf;
		SDLImage image;
		SDLMixer mixer;
		
		// Straightforward wrappers around corresponding SDL2 objects
		// These take full care of proper object destruction and error checking
		Window window("libSDL2pp demo",	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
		Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

		unordered_map<string, shared_ptr<Screen>> map;
		shared_ptr<Screen> menu(new MenuScreen);
		map.insert({ "menu", menu });

		GameManager mgr(sdl, image, mixer, sdl_ttf, window, renderer, map);
		std::string startScreen("menu");
		mgr.Loop(startScreen);
	}
	catch (SDL2pp::Exception& e) {
		// Exception stores SDL_GetError() result and name of function which failed
		std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
		std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
	}
	catch (std::exception& e) {
		// This also works (e.g. "SDL_Init failed: No available video device")
		std::cerr << e.what() << std::endl;
	}
	std::cin.get();
	return 0;
}