#include "RustyWindowsManager.h"

RustyWindowsManager::RustyWindowsManager()
{
	this->_idCounter = 1;
	this->_currentWindowId = 0;
}

unsigned int RustyWindowsManager::addWindow(RustyWindow* window)
{
	this->_windows[this->_idCounter] = window;
	this->_currentWindowId = this->_idCounter;
	this->_idCounter++;

	return this->_idCounter - 1;
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

void RustyWindowsManager::removeCurrentWindow()
{
	delete this->_windows[this->_currentWindowId];
	this->_windows.erase(this->_currentWindowId);
	
	if (this->_windows.empty()) {
		this->_currentWindowId = 0;
	}
	{
		for (auto window : this->_windows) {
			this->_currentWindowId = window.first;
		}
	}
}

RustyWindow* RustyWindowsManager::getCurrentWindow()
{
	return this->_windows[this->_currentWindowId];
}

unsigned int RustyWindowsManager::getCurrentWindowId()
{
	return this->_currentWindowId;
}

void RustyWindowsManager::updateCurrentWindow(int mousePositionX, int mousePositionY)
{
	unsigned int newCurrentWindowId = 0;
	RustyWindow* newCurrentWindow = nullptr;

	// if current window is running, end update
	if (RRW_CheckMousePositionOnObject(mousePositionX, mousePositionY, this->_windows[this->_currentWindowId]->getBarAndWindowPosition())) {
		return;
	}

	// find new current window
	for (auto window : this->_windows) {
		if (RRW_CheckMousePositionOnObject(mousePositionX, mousePositionY, window.second->getBarAndWindowPosition())) {
			newCurrentWindowId = window.first;
			newCurrentWindow = window.second;
			break;
		}
	}

	// if nothing match, dont change anything
	if (newCurrentWindow == nullptr) {
		return;
	}

	// move new current window at the top
	this->_windows.erase(newCurrentWindowId);
	this->_windows[newCurrentWindowId] = newCurrentWindow;

	// set new current window
	this->_currentWindowId = newCurrentWindowId;
}

void RustyWindowsManager::draw()
{
	for (auto window : this->_windows) {
		window.second->draw();
	}
}

bool RustyWindowsManager::isAnyWindow()
{
	return !this->_windows.empty();
}

RustyWindowsManager::~RustyWindowsManager()
{
	for (auto window : this->_windows) {
		delete window.second;
	}
}