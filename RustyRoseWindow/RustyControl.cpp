#include "RustyControl.h"

void RustyControl::addKeyFunction(SDL_Keycode key, std::function<void()> function)
{
	this->_keyFunctions[key] = function;
}

void RustyControl::handle(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        auto it = this->_keyFunctions.find(event.key.keysym.sym);
        if (it != this->_keyFunctions.end()) {
            it->second();
        }
    }
}
