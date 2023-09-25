#pragma once
#include <SDL.h>
#include <unordered_map>
#include <functional>

class RustyControl
{
public:
	void addKeyFunction(SDL_Keycode key, std::function<void()> function);
	void handle(SDL_Event event); // need more test

private:
	std::unordered_map<SDL_Keycode, std::function<void()>> _keyFunctions;
	std::vector<SDL_Keycode> _pressedKey;

	bool _isPressed(SDL_Keycode key);
};

