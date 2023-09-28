#include "RustyRenderWindow.h"

RustyRenderWindow::RustyRenderWindow(SDL_Renderer* renderer, TTF_Font* dialogFont, ScreenSize* screenSize)
{
	this->_manager = new RustyWindowsManager();
	this->_scene = new RustyScene(renderer, dialogFont, screenSize);
}

RustyWindowsManager* RustyRenderWindow::getManager()
{
	return this->_manager;
}

RustyScene* RustyRenderWindow::getScene()
{
	return this->_scene;
}

void RustyRenderWindow::draw()
{
	this->_scene->draw();
	this->_manager->draw();
}

void RustyRenderWindow::reversedDraw()
{
	this->_manager->draw();
	this->_scene->draw();
}

RustyRenderWindow::~RustyRenderWindow()
{
	delete this->_manager;
	delete this->_scene;
}
