#pragma once
#include "SharedResourceUtil.h"
#include "RustyDialogWindow.h"
#include <unordered_map>

class RustyWindowsManager
{
public:
	RustyWindowsManager();
	unsigned int addWindow(RustyWindow* window); //return id of window
	RustyWindow* getWindow(unsigned int id);
	void removeWindow(unsigned int id);
	RustyWindow* getCurrentWindow();
	void updateCurrentWindow(int mousePositionX, int mousePositionY);
	void draw();

	~RustyWindowsManager();

private:
	unsigned int _idCounter;
	unsigned int _currentWindowId;

	std::unordered_map<int, RustyWindow*> _windows;
};

