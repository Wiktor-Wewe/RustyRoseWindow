#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindowsManager.h"
#include "RustyScene.h"

class RustyRenderWindow
{
public:
	RustyRenderWindow(std::string windowName, int windowWidth, int windowHeight, const char* fontPath);
	
	void setFontSmall(const char* fontPath, int size);
	void setFontMedium(const char* fontPath, int size);
	void setFontLarge(const char* fontPath, int size);

	RustyWindowsManager* getManager();
	RustyScene* getScene();

	int getInitStatus();
	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
	ScreenSize* getScreenSize();
	Fonts* getFonts();

	void draw();
	void reversedDraw();

	~RustyRenderWindow();

private:
	int _initStatus;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	ScreenSize* _screenSize;
	Fonts* _fonts;

	RustyWindowsManager* _manager;
	RustyScene* _scene;
};

