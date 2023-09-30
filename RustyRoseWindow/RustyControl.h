#pragma once
#include "SharedResourceUtil.h"
#include <unordered_map>
#include <functional>

class RustyControl
{
public:
	void addKeyFunction(SDL_Keycode key, std::function<void()> function);
	void handle(SDL_Event event); // need more test
	void resetMove();
	RRW_MouseInfo getMouseInfo();
	RRW_MouseMove getMouseMove();

private:
	std::unordered_map<SDL_Keycode, std::function<void()>> _keyFunctions;
	std::vector<SDL_Keycode> _pressedKey;

	bool _isPressed(SDL_Keycode key);

	int _mousePositionX;
	int _mousePositionXmem;
	int _mousePositionY;
	int _mousePositionYmem;
	bool _leftClick;
	bool _rightClick;
};

