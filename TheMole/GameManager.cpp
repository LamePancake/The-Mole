#include "GameManager.h"
#include "MenuScreen.h"

using std::string;
using std::shared_ptr;
using std::unordered_map;

GameManager* GameManager::_instance = nullptr;

GameManager* GameManager::GetInstance() 
{
	return _instance;
}

void GameManager::SetNextScreen(std::string&& nextScreenName) 
{
	// nextScreenName becomes an lvalue, so we can call the version accepting a reference instead
	SetNextScreen(nextScreenName);
}

void GameManager::SetNextScreen(const std::string& nextScreenName) {
	if (nextScreenName.size() == 0) {
		_nextScreen = nullptr;
		return;
	}
	_nextScreen = _screens[nextScreenName];
}

void GameManager::ClearBackStack() {
	while (!_backStack.empty()) {
		shared_ptr<Screen> screen = _backStack.top();
		screen->Unload();
		_backStack.pop();
	}
}

const shared_ptr<Screen> GameManager::GetCurrentScreen() const
{
	return _curScreen;
}

void GameManager::OverlayScreen(shared_ptr<Screen> screen) {
	_backStack.push(screen);
	_curScreen = screen;
	_curScreen->Load();
}

void GameManager::FinishScreen() {
	// We may have called ClearBackStack earlier, so check to make sure it's not empty
	if (!_backStack.empty()) {
		_curScreen->Unload();
		_backStack.pop();
	}

	// If next screen is specified, then we're loading a new screen; otherwise, we're going back in the back stack
	if (_nextScreen) {
		_curScreen = _nextScreen;
		_nextScreen = nullptr;
        _backStack.push(_curScreen);
		_curScreen->Load();
	}
	else {
		if (_backStack.empty()) {
			// Looks like we're exiting the program
			std::cerr << "No more screens in back stack and no next screen specified. See ya!" << std::endl;
			exit(1);
		}
		// If we do have a screen though, it will already be loaded since it's on the back stack, so we can just carry on
		_curScreen = _backStack.top();
	}
}

void GameManager::Loop(string& startScreen) {
	_curScreen = _screens[startScreen];
	_curScreen->Load();
	_backStack.push(_curScreen);
	
	int currentTime = SDL_GetTicks();
	int newTime = 0;
	int dt = 0;
	int updateResult;

	while (true) {
		// Perform the update
		newTime = SDL_GetTicks();
		dt = newTime - currentTime;
		currentTime = newTime;
		
		updateResult = _curScreen->Update((dt) / 1000.0);

		switch (updateResult) {
		case Screen::SCREEN_CONTINUE:
			_curScreen->Draw();
			break;
		// Because we're calling Load in these two, we'll need to update currentTime so that there isn't a huge delay
		case Screen::SCREEN_OVERLAY:
			OverlayScreen(_nextScreen);
			currentTime = SDL_GetTicks();
			break;
		case Screen::SCREEN_FINISH:
			FinishScreen();
			currentTime = SDL_GetTicks();
			break;
		}
		SDL_Delay(1);
	}
}

void GameManager::ReadLevelUnlockFile(std::string path)
{
	std::string line;
	std::ifstream inFile(path.c_str());
	std::string levels[] = { "den", "viking", "grass", "starscape", "boss", "back" };

	if (!inFile.is_open())
	{
		_unlockedLevels["den"] = true;
		_unlockedLevels["viking"] = false;
		_unlockedLevels["grass"] = false;
		_unlockedLevels["starscape"] = false;
		_unlockedLevels["boss"] = false;
		_unlockedLevels["back"] = true;
		return;
	}

	std::getline(inFile, line);

	if (line.size() > 6)
	{
		_unlockedLevels["den"] = true;
		_unlockedLevels["viking"] = false;
		_unlockedLevels["grass"] = false;
		_unlockedLevels["starscape"] = false;
		_unlockedLevels["boss"] = false;
		_unlockedLevels["back"] = true;
		return;
	}

	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i] == '1')
			_unlockedLevels[levels[i]] = true;
		else
			_unlockedLevels[levels[i]] = false;
	}

	inFile.close();
	_unlockedLevels["den"] = true;
	_unlockedLevels["back"] = true;
}

void GameManager::WriteLevelUnlockFile(std::string path)
{
	std::ofstream myfile;
	myfile.open(path);
	
	if (myfile.is_open())
	{
		myfile << _unlockedLevels["den"] ? "1" : "0";
		myfile << _unlockedLevels["viking"] ? "1" : "0";
		myfile << _unlockedLevels["grass"] ? "1" : "0";
		myfile << _unlockedLevels["starscape"] ? "1" : "0";
		myfile << _unlockedLevels["boss"] ? "1" : "0";
		myfile << _unlockedLevels["back"] ? "1" : "0";
	}
	
	myfile.close();
}

void GameManager::ReadHighScoreFile(std::string path)
{
	std::string line;
	std::ifstream inFile(path.c_str());

	if (!inFile.is_open())
	{
		_bestDeathCount = 999;
		_bestPancakeCount = 0;
		return;
	}

	std::getline(inFile, line);
	_bestDeathCount = atoi(line.c_str());

	std::getline(inFile, line);
	_bestPancakeCount = atoi(line.c_str());

	inFile.close();
}

void GameManager::WriteHighScoreFile(std::string path)
{
	std::ofstream myfile;
	myfile.open(path);

	if (myfile.is_open())
	{
		myfile << _bestDeathCount << std::endl;
		myfile << _bestPancakeCount << std::endl;
	}

	myfile.close();
}

void GameManager::ClearHighScores()
{
	_bestDeathCount = 999;
	_bestPancakeCount = 0;
}

void GameManager::ClearSavedData()
{
	std::string levelScores[NUM_LEVELS] =
	{
		".\\Assets\\SavedData\\den_score.txt",
		".\\Assets\\SavedData\\viking_score.txt",
		".\\Assets\\SavedData\\grasslands_score.txt",
		".\\Assets\\SavedData\\starscape_score.txt",
		".\\Assets\\SavedData\\boss_score.txt",
	};

	std::ofstream levelUnlock;
	levelUnlock.open(".\\Assets\\SavedData\\level_unlocks.txt");

	if (levelUnlock.is_open())
	{
		levelUnlock << 1;
		levelUnlock << 0;
		levelUnlock << 0;
		levelUnlock << 0;
		levelUnlock << 0;
		levelUnlock << 1;
	}

	levelUnlock.close();

	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		std::ofstream myfile;
		myfile.open(levelScores[i]);

		if (myfile.is_open())
		{
			myfile << 999 << std::endl;
			myfile << 0 << std::endl;
		}

		myfile.close();
	}
}
