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
#include "OptionsScreen.h"


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
		soundEffects["dig"] = "./Assets/Audio/digging.ogg";
        soundEffects["door_finish"] = "./Assets/Audio/door_finish.ogg";
        soundEffects["door_progress"] = "./Assets/Audio/door_progress.ogg";
        soundEffects["switch_on"] = "./Assets/Audio/switch_on.ogg";
		soundEffects["select"] = "./Assets/Audio/menu_selection.ogg";
		soundEffects["accept"] = "./Assets/Audio/menu_accept.ogg";
		soundEffects["decline"] = "./Assets/Audio/menu_decline.ogg";
		soundEffects["warning"] = "./Assets/Audio/menu_warning.ogg";
		soundEffects["borin1"] = "./Assets/Audio/borin_voice1.ogg";
		soundEffects["borin2"] = "./Assets/Audio/borin_voice2.ogg";
		soundEffects["borin3"] = "./Assets/Audio/borin_voice3.ogg";
		soundEffects["alien1"] = "./Assets/Audio/alien_voice1.ogg";
		soundEffects["alien2"] = "./Assets/Audio/alien_voice2.ogg";
		soundEffects["alien3"] = "./Assets/Audio/alien_voice3.ogg";
		soundEffects["viking1"] = "./Assets/Audio/viking_voice1.ogg";
		soundEffects["viking2"] = "./Assets/Audio/viking_voice2.ogg";
		soundEffects["viking3"] = "./Assets/Audio/viking_voice3.ogg";
		soundEffects["toad1"] = "./Assets/Audio/toad_voice1.ogg";
		soundEffects["toad2"] = "./Assets/Audio/toad_voice2.ogg";
		soundEffects["toad3"] = "./Assets/Audio/toad_voice3.ogg";
		SoundEffectBank bank(soundEffects);

		vector<string> borinVoices = { "borin1", "borin2", "borin3" };
		vector<string> alienVoices = { "alien1", "alien2", "alien3" };
		vector<string> vikingVoices = { "viking1", "viking2", "viking3" };
		vector<string> toadVoices = { "toad1", "toad2", "toad3" };
			
		// Straightforward wrappers around corresponding SDL2 objects
		// These take full care of proper object destruction and error checking
		Window window("The Mole", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_RESIZABLE);//SDL_WINDOW_FULLSCREEN);
		Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

		unordered_map<string, shared_ptr<Screen>> screens;
		shared_ptr<Screen> menu(new MenuScreen(bank));
		shared_ptr<Screen> levelSelect(new LevelSelectScreen(bank));

		shared_ptr<Screen> den(new GameScreen("./Assets/Levels/den_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/den_score.txt", "viking", bank));
		shared_ptr<Screen> viking(new GameScreen("./Assets/Levels/viking_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/viking_score.txt", "grass", bank));
		shared_ptr<Screen> grass(new GameScreen("./Assets/Levels/grass_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/grasslands_score.txt", "starscape", bank));
		shared_ptr<Screen> starscape(new GameScreen("./Assets/Levels/starscape_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/starscape_score.txt", "boss", bank));
		shared_ptr<Screen> boss(new GameScreen("./Assets/Levels/boss_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/boss_score.txt", "levelSelect", bank));
		
		shared_ptr<Screen> denIntro(new CutsceneScreen("./Assets/Textures/dialog_box.png", "./Assets/Textures/den_bg.png", "./Assets/Textures/Borin_den_dialog.png", 4, 0.8, "./Assets/Textures/Toad_dialog.png", 6, 0.50, bank, "denpost", "./Assets/Dialog/den_dialog.txt", "Borin", "Toad", borinVoices, toadVoices));
		shared_ptr<Screen> vikingIntro(new CutsceneScreen("./Assets/Textures/dialog_box.png", "./Assets/Textures/den_bg.png", "./Assets/Textures/Borin_dialog.png", 4, 0.8, "./Assets/Textures/Adalbert_dialog.png", 4, 0.8, bank, "vikingpost", "./Assets/Dialog/viking_dialog.txt", "Borin", "Adalbert", borinVoices, vikingVoices));
		shared_ptr<Screen> grassIntro(new CutsceneScreen("./Assets/Textures/dialog_box.png", "./Assets/Textures/den_bg.png", "./Assets/Textures/Borin_dialog.png", 4, 0.8, "./Assets/Textures/Cornelius_dialog.png", 4, 0.8, bank, "grasspost", "./Assets/Dialog/grassland_dialog.txt", "Borin", "Cornelius", borinVoices, vikingVoices));
		shared_ptr<Screen> starscapeIntro(new CutsceneScreen("./Assets/Textures/dialog_box.png", "./Assets/Textures/den_bg.png", "./Assets/Textures/Borin_dialog.png", 4, 0.8, "./Assets/Textures/Adalbert_dialog.png", 4, 0.8, bank, "starscapepost", "./Assets/Dialog/starscape_dialog.txt", "Borin", "Adalbert", borinVoices, vikingVoices));
		shared_ptr<Screen> bossIntro(new CutsceneScreen("./Assets/Textures/dialog_box.png", "./Assets/Textures/den_bg.png", "./Assets/Textures/Borin_dialog.png", 4, 0.8, "./Assets/Textures/Watch_dialog.png", 4, 0.8, bank, "bosslevel", "./Assets/Dialog/final_battle_dialog.txt", "Borin", "The Underwatch", borinVoices, alienVoices));

		// The Pre Dialog Intro screens
		shared_ptr<Screen> denPreIntro(new GameScreen("./Assets/Levels/preden_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/den_score.txt", "denintro", bank, false));
		shared_ptr<Screen> vikingPreIntro(new GameScreen("./Assets/Levels/previking_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/viking_score.txt", "vikingintro", bank, false));
		shared_ptr<Screen> grassPreIntro(new GameScreen("./Assets/Levels/pregrass_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/grasslands_score.txt", "grassintro", bank, false));
		shared_ptr<Screen> starscapePreIntro(new GameScreen("./Assets/Levels/prestarscape_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/starscape_score.txt", "starscapeintro", bank, false));
		shared_ptr<Screen> bossPreIntro(new GameScreen("./Assets/Levels/preboss_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/boss_score.txt", "bossintro", bank, false));

		// The Post Dialog Intro screens
		// 
		shared_ptr<Screen> denPostIntro(new GameScreen("./Assets/Levels/postden_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/den_score.txt", "denlevel", bank, false));
		shared_ptr<Screen> vikingPostIntro(new GameScreen("./Assets/Levels/postviking_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/viking_score.txt", "vikinglevel", bank, false));
		shared_ptr<Screen> grassPostIntro(new GameScreen("./Assets/Levels/postgrass_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/grasslands_score.txt", "grasslevel", bank, false));
		shared_ptr<Screen> starscapePostIntro(new GameScreen("./Assets/Levels/poststarscape_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/starscape_score.txt", "starscapelevel", bank, false));
		shared_ptr<Screen> bossPostIntro(new GameScreen("./Assets/Levels/postboss_level.txt", "./Assets/Textures/den_bg.png", "./Assets/SavedData/boss_score.txt", "bosslevel", bank, false));

		shared_ptr<Screen> credits(new CreditsScreen(bank));
		shared_ptr<Screen> controls(new ControlsScreen(bank));
		shared_ptr<Screen> options(new OptionsScreen(bank));

		screens.insert({ "menu", menu });
		screens.insert({ "levelSelect", levelSelect });
		screens.insert({ "credits", credits });
		screens.insert({ "controls", controls });
		screens.insert({ "options", options });

		screens.insert({ "denlevel", den });
		screens.insert({ "vikinglevel", viking });
		screens.insert({ "grasslevel", grass });
		screens.insert({ "starscapelevel", starscape });
		screens.insert({ "bosslevel", boss });
		
		screens.insert({ "denintro", denIntro });
		screens.insert({ "vikingintro", vikingIntro });
		screens.insert({ "grassintro", grassIntro });
		screens.insert({ "starscapeintro", starscapeIntro });
		screens.insert({ "bossintro", bossIntro });
		
		screens.insert({ "den", denPreIntro });
		screens.insert({ "viking", vikingPreIntro });
		screens.insert({ "grass", grassPreIntro });
		screens.insert({ "starscape", starscapePreIntro });
		screens.insert({ "boss", bossPreIntro });

		screens.insert({ "denpost", denPostIntro });
		screens.insert({ "vikingpost", vikingPostIntro });
		screens.insert({ "grasspost", grassPostIntro });
		screens.insert({ "starscapepost", starscapePostIntro });
		screens.insert({ "bosspost", bossPostIntro });

		GameManager::_instance = new GameManager(std::move(sdl), std::move(image), std::move(mixer), std::move(sdl_ttf), std::move(window), std::move(renderer), screens);
        bank.LoadSounds();
		GameManager::_instance->Loop(startScreen);
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
    delete GameManager::_instance;
	return 0;
}