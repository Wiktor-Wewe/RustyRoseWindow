#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindow.h"

class RustyDialogWindow : public RustyWindow
{
public:
	RustyDialogWindow(std::string text, SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font, RRW_Font* buttonFont, int width, int height);
	void setButtonFont(RRW_Font* font);

	~RustyDialogWindow();

private:
	unsigned int _buttonId1;
	unsigned int _buttonId2;
	RRW_Font* buttonFont;

};

