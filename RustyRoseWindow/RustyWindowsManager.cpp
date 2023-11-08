#include "RustyWindowsManager.h"

RustyWindowsManager::RustyWindowsManager(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font)
{
	this->_idCounter = 1;
	this->_currentWindowId = 0;
	this->_font = font;

	this->_renderer = renderer;
	this->_screenSize = screenSize;
}

void RustyWindowsManager::setFont(RRW_Font* font)
{
	this->_font = font;
}

RustyWindow* RustyWindowsManager::makeWindow(int width, int height, int& id)
{
	RustyWindow* window = new RustyWindow(this->_renderer, this->_screenSize, this->_font, width, height);
	this->_windows.push_back(std::make_pair(this->_idCounter, window));
	this->_currentWindowId = this->_idCounter;
	id = this->_currentWindowId;
	this->_idCounter++;

	return window;
}

unsigned int RustyWindowsManager::addWindow(RustyWindow* window)
{
	if (window->isFontSet() == false) {
		window->setFont(this->_font);
	}

	this->_windows.push_back(std::make_pair(this->_idCounter, window));

	this->_currentWindowId = this->_idCounter;
	this->_idCounter++;

	return this->_idCounter - 1;
}

RustyWindow* RustyWindowsManager::getWindow(unsigned int id)
{
	for (auto w : this->_windows) {
		if (w.first == id) {
			return w.second;
		}
	}

	return nullptr;
}

void RustyWindowsManager::removeWindow(unsigned int id)
{
	for (int i = 0; i < this->_windows.size(); i++) {
		if (this->_windows[i].first == id) {
			delete this->_windows[i].second;
			this->_windows.erase(this->_windows.begin() + i);
			break;
		}
	}
	
	if (this->_windows.empty()) {
		this->_currentWindowId = 0;
	}
	{
		for (auto window : this->_windows) {
			this->_currentWindowId = window.first;
		}
	}
}

void RustyWindowsManager::removeCurrentWindow()
{

	for (int i = 0; i < this->_windows.size(); i++) {
		if (this->_windows[i].first == this->_currentWindowId) {
			delete this->_windows[i].second;
			this->_windows.erase(this->_windows.begin() + i);
			break;
		}
	}
	
	if (this->_windows.empty()) {
		this->_currentWindowId = 0;
	}
	else {
		for (auto window : this->_windows) {
			this->_currentWindowId = window.first;
		}
	}
}

RustyWindow* RustyWindowsManager::getCurrentWindow()
{
	for (auto window : this->_windows) {
		if (window.first == this->_currentWindowId) {
			return window.second;
		}
	}

	return nullptr;
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
	if (RRW_CheckMousePositionOnObject(mousePositionX, mousePositionY, this->getCurrentWindow()->getBarAndWindowPosition())) {
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
	for (int i = 0; i < this->_windows.size(); i++) {
		if (this->_windows[i].first == newCurrentWindowId) {
			this->_windows.erase(this->_windows.begin() + i);
			break;
		}
	}

	this->_windows.push_back(std::make_pair(newCurrentWindowId, newCurrentWindow));

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