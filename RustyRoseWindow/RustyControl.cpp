#include "RustyControl.h"

void RustyControl::addKeyFunction(SDL_Keycode key, std::function<void()> function)
{
	this->_keyFunctions[key] = function;
}

void RustyControl::handle(SDL_Event event)
{
    // if some of the key that have function are pressed add it to this->_pressedKey
    if (event.type == SDL_KEYDOWN) {
        auto it = this->_keyFunctions.find(event.key.keysym.sym);
        if (it != this->_keyFunctions.end()) {
            if (this->_isPressed(it->first) == false) {
                this->_pressedKey.push_back(it->first);
            }
        }
    }

    // remove key from list if KEYUP
    if (event.type == SDL_KEYUP) {
        auto it = std::find(this->_pressedKey.begin(), this->_pressedKey.end(), event.key.keysym.sym);
        if (it != this->_pressedKey.end()) {
            this->_pressedKey.erase(it);
        }
    }

    // handle all of this->_pressedKey
    for (auto key : this->_pressedKey) {
        this->_keyFunctions[key]();
    }
}

bool RustyControl::_isPressed(SDL_Keycode key)
{
    auto it = std::find(this->_pressedKey.begin(), this->_pressedKey.end(), key);
    if (it != this->_pressedKey.end()) {
        return true;
    }
    return false;
}
