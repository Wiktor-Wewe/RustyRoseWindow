#pragma once
#include <SDL.h>
#include <unordered_map>
#include <functional>

class RustyControl
{
public:
	void addKeyFunction(SDL_Keycode key, std::function<void()> function);
	void handle(SDL_Event event);

private:
	std::unordered_map<SDL_Keycode, std::function<void()>> _keyFunctions;
	
};

