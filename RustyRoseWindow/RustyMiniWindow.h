#pragma once
#include "SharedResourceUtil.h"
#include "RustyButton.h"

class RustyMiniWindow
{
public :
	RustyMiniWindow();

	~RustyMiniWindow();

private:
	SDL_Renderer* _renderer;
	int _selectIndex;
	bool _end;

	SDL_Texture* _mainTexture; // main texture without hover
	SDL_Texture* _mainTextureHover; // only hover to main texture



};

