#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <SDL2pp\SDL2pp.hh>

// Forward declaration since GameManager.h includes Screen.h
class GameManager;

class Screen {

public:
	/**
	 * Load any time-consuming stuff (e.g. textures, audio clips, etc.) here.
	 * Note that the game manager isn't a singleton (it might become one later), so for now, be sure to save a (const) reference to it somewhere.
	 *
	 * @return SCREEN_LOAD_SUCCESS or SCREEN_LOAD_FAILURE to indicate whether loading was successful. Not really used right now.
	 */
	virtual int Load() = 0;

	/**
	 * Updates the current screen.
	 * Note: the GameManager will NOT call PumpEvents, PollEvent, etc.; it's up to the Screen to call this. Not doing so will result in no updates!
	 *
	 * The return value from this function determines when this screen is unloaded and allows for overlaying of other screens.
	 * SCREEN_CONTINUE tells the game manager to keep updating this screen as normal. SCREEN_OVERLAY tells the GameManager to keep this screen loaded
	 * but to load another screen (which must be set with GameManager::SetNextScreen) on top of it. SCREEN_FINISH tells the GameManager to unload this screen
	 * and either load the previous screen on the back stack (by calling GameManager::SetNextScreen(nullptr)) or to load a specific screen set with
	 * GameManager::SetNextScreen(name). Screens that might be overlaid (e.g. settings menus) might also want to call GameManager::ClearBackStack if they're
	 * loading a new screen which won't return to screens in the back stack. For instance, a level selection menu should call ClearBackStack if it was overlaid on
	 * the main menu and someone selects "Play" directly from there so that the main menu screen's resources are also unloaded before going to the level.
	 *
	 * @param elapsedSecs: The number of seconds that elapsed since the last update.
	 * @return SCREEN_CONTINUE, SCREEN_OVERLAY or SCREEN_FINISH as described above.
	 */
	virtual int Update(double elapsedSecs) = 0;

	/**
	 * Perform all rendering operations here.
	 * You'll need to use GameManager::GetRenderer to draw anything.
	 */
	virtual void Draw() = 0;

	/**
	 * Unload any large resources (probably everything loaded in Load) here. Next time the screen is loaded, it will have the opportunity to load it again.
	 */
	virtual void Unload() = 0;

	enum {
		SCREEN_CONTINUE,
		SCREEN_OVERLAY,
		SCREEN_FINISH
	};

	enum {
		SCREEN_LOAD_SUCCESS,
		SCREEN_LOAD_FAILTURE
	};
};

#endif