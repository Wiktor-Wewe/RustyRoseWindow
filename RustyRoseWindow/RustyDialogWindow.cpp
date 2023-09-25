#include "RustyDialogWindow.h"

RustyDialogWindow::RustyDialogWindow(std::string text, SDL_Renderer* renderer, ScreenSize* screenSize, TTF_Font* font, TTF_Font* buttonFont, int width, int height) :
	RustyWindow(renderer, screenSize, font, width, height) 
{
	this->buttonFont = buttonFont;

	this->addText(text, 0, 0, this->_font);
	this->_buttonId1 = this->addButton("Yes", 10, 300, 80, 20, this->buttonFont);
	this->_buttonId2 = this->addButton("No", 120, 300, 80, 20, this->buttonFont);
}

void RustyDialogWindow::setButtonFont(TTF_Font* font)
{
	this->buttonFont = font;
}

RustyDialogWindow::~RustyDialogWindow()
{
	
}