#include "RustyDialogWindow.h"

RustyDialogWindow::RustyDialogWindow(std::string text, SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font, RRW_Font* buttonFont, int width, int height) :
	RustyWindow(renderer, screenSize, font, width, height) 
{
	this->buttonFont = buttonFont;
	this->addText(text, 0, 0, this->_font);

	this->_buttonId1 = this->addButton("Yes", 0, 0, 80, 20, this->buttonFont);
	this->_buttonId2 = this->addButton("No", 0, 0, 80, 20, this->buttonFont);
	this->_buttonId2 = this->addButton("Cancel", 0, 0, 80, 20, this->buttonFont);
	this->centerButtons();
	this->centerTexts();
}

void RustyDialogWindow::setButtonFont(RRW_Font* font)
{
	this->buttonFont = font;
}

RustyDialogWindow::~RustyDialogWindow()
{
	
}