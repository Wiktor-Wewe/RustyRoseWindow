#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindowsManager.h"
#include "RustyScene.h"

class RustyRenderWindow
{
public:
	RustyRenderWindow(SDL_Renderer* renderer, TTF_Font* dialogFont, ScreenSize* screenSize);
	RustyWindowsManager* getManager();
	RustyScene* getScene();

	void draw();
	void reversedDraw();

	~RustyRenderWindow();

private:
	RustyWindowsManager* _manager;
	RustyScene* _scene;

};

