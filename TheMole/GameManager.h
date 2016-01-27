#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <memory>
#include <SDL2pp\SDL2pp.hh>
#include "Screen.h"
#include "LevelLoader.h"
#include "Level.h"
#include "LevelRenderer.h"
#include <vector>

class GameManager {
public:
	/**
	 * Creates a new GameManager object with references to the SDL libraries, a window, and the map of screens to be used in the game.
	 *
	 */
	GameManager(SDL2pp::SDL& sdl, SDL2pp::SDLImage& sdlImage, SDL2pp::SDLMixer& sdlMixer, SDL2pp::SDLTTF& sdlTtf,
		SDL2pp::Window& window, SDL2pp::Renderer& renderer, std::unordered_map<std::string, std::shared_ptr<Screen>>& screens) 
	:_sdl(sdl), _sdlImage(sdlImage), _sdlMixer(sdlMixer), _sdlTtf(sdlTtf),
		_window(window), _renderer(renderer), _screens(screens) {
	}

	// Don't want the GameManager object to be copied or moved
	GameManager operator=(GameManager&) = delete;
	GameManager(GameManager&) = delete;

	/**
	 * Sets the next screen to be loaded.
	 *
	 * @param nextScreenName The name of the screen to be loaded.
	 */
	void SetNextScreen(const char* nextScreenName);

	/**
	 * Starts the game loop with the specified starting screen.
	 */
	void Loop(std::string& startScreen);

	/**
	 * Clears the back stack 
	 */
	void ClearBackStack();

	// SDL accessor methods
	SDL2pp::SDL& GetSDL() const { return _sdl; }
	SDL2pp::SDLImage& GetSDLImage() const { return _sdlImage; }
	SDL2pp::SDLMixer& GetSDLMixer() const { return _sdlMixer; }
	SDL2pp::SDLTTF& GetSDLTTF() const { return _sdlTtf; }
	SDL2pp::Window& GetWindow() const { return _window; }
	SDL2pp::Renderer& GetRenderer() const { return _renderer; }

private:
	/**
	 * Overlays the given screen on top of the current one.
	 * The new screen will have had its Load() method called on return from this method.
	 *
	 * @param name: The name of the screen to be overlaid.
	 */
	void OverlayScreen(std::shared_ptr<Screen> name);

	/**
	 * Unloads the current screen and loads the next one as specified by _nextScreen.
	 */
	void FinishScreen();

	// The screens in the game
	std::unordered_map<std::string, std::shared_ptr<Screen>>& _screens;
	std::stack<std::shared_ptr<Screen>> _backStack;
	std::shared_ptr<Screen> _nextScreen;
	std::shared_ptr<Screen> _curScreen;

	SDL2pp::SDL& _sdl;
	SDL2pp::SDLImage& _sdlImage;
	SDL2pp::SDLMixer& _sdlMixer;
	SDL2pp::SDLTTF& _sdlTtf;
	SDL2pp::Window& _window;
	SDL2pp::Renderer& _renderer;
};