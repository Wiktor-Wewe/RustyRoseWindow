#pragma once
#include "SharedResourceUtil.h"
#include "RustyDialogWindow.h"
#include <unordered_map>

class RustyWindowsManager
{
public:
	RustyWindowsManager(SDL_Renderer* renderer);
	void addWindow(RustyWindow* window);
	void draw();

	~RustyWindowsManager();

private:
	SDL_Renderer* _renderer;
	int _currentWindowId;

	std::vector<RustyWindow*> _windows;
	//std::unordered_map<int, RustyDialogWindow*> _dialogWindows;
};

