#include "RustyMiniWindow.h"

void RustyMiniWindow::setPosition(int x, int y)
{
	this->_position->x = x;
	this->_position->y = y;
}

void RustyMiniWindow::setSize(int width, int height)
{
	this->_position->w = width;
	this->_position->h = height;
}

void RustyMiniWindow::setDefaultFont(TTF_Font* font)
{
	this->_defaultFont = font;
}

void RustyMiniWindow::setBackGroundColor(SDL_Color color)
{
	this->_backGroundColor = color;
}

void RustyMiniWindow::setWindowHoverColor(SDL_Color color)
{
	this->_windowHoverColor = color;
}

void RustyMiniWindow::setFontColor(SDL_Color color)
{
	this->_fontColor = color;
}

void RustyMiniWindow::setFontHoverColor(SDL_Color color)
{
	this->_fontHoverColor = color;
}
