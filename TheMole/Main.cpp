#include <iostream>
#include <unordered_map>
#include <string>
#include <SDL2pp\SDL2pp.hh>
#include <SDL2\SDL_main.h>
#include <memory>
#include "GameManager.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "LevelSelectScreen.h"
#include "Input.h"
#include "CutsceneScreen.h"
#include "CreditsScreen.h"
#include "ControlsScreen.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;

int main(int argc, char** argv) {
	try {
		std::string startScreen("menu");

		using namespace SDL2pp;
		uint32_t subsystems = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER;
		SDL sdl(subsystems);
		SDLTTF sdl_ttf;
		SDLImage image;
		SDLMixer sdlMixer(MIX_INIT_OGG | MIX_INIT_MP3);
        Mixer mixer(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		unordered_map<string, string> soundEffects;
		soundEffects["dig"] = ".\\Assets\\Audio\\digging.ogg";
		SoundEffectBank bank(soundEffects);

		// Straightforward wrappers around corresponding SDL2 objects
		// These take full care of proper object destruction and error checking
		Window window("The Mole", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_RESIZABLE);//SDL_WINDOW_FULLSCREEN);
		Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

		unordered_map<string, shared_ptr<Screen>> screens;
		shared_ptr<Screen> menu(new MenuScreen);
		shared_ptr<Screen> levelSelect(new LevelSelectScreen);
		
		shared_ptr<Screen> den(new GameScreen(".\\Assets\\Levels\\den_level.txt", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\GUI\\Level_Complete\\level_cleared_keyboard.png", ".\\Assets\\GUI\\Level_Complete\\level_failed_keyboard.png", "viking", bank));
		shared_ptr<Screen> viking(new GameScreen(".\\Assets\\Levels\\viking_level.txt", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\GUI\\Level_Complete\\level_cleared_keyboard.png", ".\\Assets\\GUI\\Level_Complete\\level_failed_keyboard.png", "grass", bank));
		shared_ptr<Screen> grass(new GameScreen(".\\Assets\\Levels\\grass_level.txt", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\GUI\\Level_Complete\\level_cleared_keyboard.png", ".\\Assets\\GUI\\Level_Complete\\level_failed_keyboard.png", "starscape", bank));
		shared_ptr<Screen> starscape(new GameScreen(".\\Assets\\Levels\\starscape_level.txt", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\GUI\\Level_Complete\\level_cleared_keyboard.png", ".\\Assets\\GUI\\Level_Complete\\level_failed_keyboard.png", "boss", bank));
		shared_ptr<Screen> boss(new GameScreen(".\\Assets\\Levels\\boss_level.txt", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\GUI\\Level_Complete\\level_cleared_keyboard.png", ".\\Assets\\GUI\\Level_Complete\\level_failed_keyboard.png", "levelSelect", bank));
		
		shared_ptr<Screen> test(new GameScreen(".\\Assets\\Levels\\test_level.txt", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\GUI\\Level_Complete\\level_cleared_keyboard.png", ".\\Assets\\GUI\\Level_Complete\\level_failed_keyboard.png", "menu", bank));
		
		shared_ptr<Screen> denIntro(new CutsceneScreen(".\\Assets\\GUI\\dialog_box.png", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\Textures\\den_bg.png", 1, 0, ".\\Assets\\Textures\\Borin_dialog.png", 4, 0.8, ".\\Assets\\Textures\\Toad_dialog.png", 6, 0.50, bank, "denlevel", ".\\Assets\\Dialog\\den_dialog.txt", "Borin", "Toad"));
		shared_ptr<Screen> vikingIntro(new CutsceneScreen(".\\Assets\\GUI\\dialog_box.png", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\Textures\\den_bg.png", 1, 0, ".\\Assets\\Textures\\Borin_dialog.png", 4, 0.8, ".\\Assets\\Textures\\Toad_dialog.png", 6, 0.50, bank, "vikinglevel", ".\\Assets\\Dialog\\viking_dialog.txt", "Borin", "Viking"));
		shared_ptr<Screen> grassIntro(new CutsceneScreen(".\\Assets\\GUI\\dialog_box.png", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\Textures\\den_bg.png", 1, 0, ".\\Assets\\Textures\\Borin_dialog.png", 4, 0.8, ".\\Assets\\Textures\\Toad_dialog.png", 6, 0.50, bank, "grasslevel", ".\\Assets\\Dialog\\grassland_dialog.txt", "Borin", "Viking"));
		shared_ptr<Screen> starscapeIntro(new CutsceneScreen(".\\Assets\\GUI\\dialog_box.png", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\Textures\\den_bg.png", 1, 0, ".\\Assets\\Textures\\Borin_dialog.png", 4, 0.8, ".\\Assets\\Textures\\Toad_dialog.png", 6, 0.50, bank, "starscapelevel", ".\\Assets\\Dialog\\starscape_dialog.txt", "Borin", "Viking"));
		shared_ptr<Screen> bossIntro(new CutsceneScreen(".\\Assets\\GUI\\dialog_box.png", ".\\Assets\\Textures\\den_bg.png", ".\\Assets\\Textures\\den_bg.png", 1, 0, ".\\Assets\\Textures\\Borin_dialog.png", 4, 0.8, ".\\Assets\\Textures\\Toad_dialog.png", 6, 0.50, bank, "bosslevel", ".\\Assets\\Dialog\\final_battle_dialog.txt", "Borin", "Gray"));

		shared_ptr<Screen> credits(new CreditsScreen());
		shared_ptr<Screen> controls(new ControlsScreen());

		screens.insert({ "menu", menu });
		screens.insert({ "levelSelect", levelSelect });
		screens.insert({ "credits", credits });
		screens.insert({ "controls", controls });

		screens.insert({ "denlevel", den });
		screens.insert({ "vikinglevel", viking });
		screens.insert({ "grasslevel", grass });
		screens.insert({ "starscapelevel", starscape });
		screens.insert({ "bosslevel", boss });
		screens.insert({ "testlevel", test });
		
		screens.insert({ "den", denIntro });
		screens.insert({ "viking", vikingIntro });
		screens.insert({ "grass", grassIntro });
		screens.insert({ "starscape", starscapeIntro });
		screens.insert({ "boss", bossIntro });

		GameManager::_instance = new GameManager(std::move(sdl), std::move(image), std::move(mixer), std::move(sdl_ttf), std::move(window), std::move(renderer), screens);
		GameManager::_instance->Loop(startScreen);
	}
	catch (SDL2pp::Exception& e) {
		// Exception stores SDL_GetError() result and name of function which failed
		std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
		std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
		cin.get();
	}
	catch (std::exception& e) {
		// This also works (e.g. "SDL_Init failed: No available video device")
		std::cerr << e.what() << std::endl;
	}
	return 0;
}