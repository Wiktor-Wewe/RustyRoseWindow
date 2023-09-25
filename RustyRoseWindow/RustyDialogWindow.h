#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindow.h"

class RustyDialogWindow : public RustyWindow
{
public:
	RustyDialogWindow(std::string text, SDL_Renderer* renderer, ScreenSize* screenSize, TTF_Font* font, TTF_Font* buttonFont, int width, int height);
	void setButtonFont(TTF_Font* font);

	~RustyDialogWindow();

private:
	unsigned int _buttonId1;
	unsigned int _buttonId2;
	TTF_Font* buttonFont;

};

