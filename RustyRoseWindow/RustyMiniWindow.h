#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindow.h"

class RustyMiniWindow : public RustyWindow
{
public :
	explicit RustyMiniWindow(SDL_Renderer* renderer, ScreenSize* screenSize, TTF_Font* font, int width, int height)
		: RustyWindow(renderer, screenSize, font, width, height) {};

	void selectId(int id);//?
	void draw() override;

	~RustyMiniWindow();

private:
	SDL_Texture* _texture;
	SDL_Texture* _customTexture;
};

