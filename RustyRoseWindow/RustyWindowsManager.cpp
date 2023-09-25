#include "RustyWindowsManager.h"

RustyWindowsManager::RustyWindowsManager(SDL_Renderer* renderer)
{
	this->_renderer = renderer;
}

void RustyWindowsManager::addWindow(RustyWindow* window)
{
	this->_windows.push_back(window);
}

void RustyWindowsManager::draw()
{
	for (auto window : this->_windows) {
		window->draw();
	}
}

RustyWindowsManager::~RustyWindowsManager()
{
}
