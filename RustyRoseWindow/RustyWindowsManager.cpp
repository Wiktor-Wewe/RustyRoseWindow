#include "RustyWindowsManager.h"

RustyWindowsManager::RustyWindowsManager()
{
	this->_idCounter = 1;
	this->_currentWindowId = 0;
}

unsigned int RustyWindowsManager::addWindow(RustyWindow* window)
{
	this->_windows[this->_idCounter] = window;
	this->_idCounter++;

	return this->_idCounter -= 1;
}

RustyWindow* RustyWindowsManager::getWindow(unsigned int id)
{
	return this->_windows[id];
}

void RustyWindowsManager::removeWindow(unsigned int id)
{
	auto response = this->_windows[id];
	if (response) {
		delete response;
		this->_windows.erase(id);
	}
}

RustyWindow* RustyWindowsManager::getCurrentWindow()
{
	return this->_windows[this->_currentWindowId];
}

void RustyWindowsManager::draw()
{
	for (auto window : this->_windows) {
		window.second->draw();
	}
}

RustyWindowsManager::~RustyWindowsManager()
{
}
