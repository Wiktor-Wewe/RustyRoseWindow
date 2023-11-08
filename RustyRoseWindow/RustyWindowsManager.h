#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindow.h"

class RustyWindowsManager
{
public:
	RustyWindowsManager(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font);
	void setFont(RRW_Font* font);

	RustyWindow* makeWindow(int width, int height, int& id);
	unsigned int addWindow(RustyWindow* window); //return id of window
	RustyWindow* getWindow(unsigned int id);

	void removeWindow(unsigned int id);
	void removeCurrentWindow();

	RustyWindow* getCurrentWindow();
	unsigned int getCurrentWindowId();

	void updateCurrentWindow(int mousePositionX, int mousePositionY);
	void draw();
	bool isAnyWindow();

	~RustyWindowsManager();

private:
	unsigned int _idCounter;
	unsigned int _currentWindowId;
	RRW_Font* _font;
	SDL_Renderer* _renderer;
	RRW_ScreenSize* _screenSize;

	std::vector<std::pair<int, RustyWindow*>> _windows;
};
