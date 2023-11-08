#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindowsManager.h"
#include "RustyScene.h"

class RustyRenderWindow
{
public:
	RustyRenderWindow(std::string windowName, int windowWidth, int windowHeight, const char* fontPath, int outlineSize = 1);
	
	void setFontSmall(const char* fontPath, int size, int outlineSize);
	void setFontMedium(const char* fontPath, int size, int outlineSize);
	void setFontLarge(const char* fontPath, int size, int outlineSize);

	RustyWindowsManager* getManager();
	RustyScene* getScene();

	int getInitStatus();
	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
	RRW_ScreenSize* getScreenSize();
	RRW_Fonts* getFonts();

	void draw();
	void reversedDraw();

	~RustyRenderWindow();

private:
	int _initStatus;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	RRW_ScreenSize* _screenSize;
	RRW_Fonts* _fonts;

	RustyWindowsManager* _manager;
	RustyScene* _scene;
};

