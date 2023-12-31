#include "RustyControl.h"

void RustyControl::addKeyFunction(SDL_Keycode key, std::function<void()> function)
{
	this->_keyFunctions[key] = function;
}

void RustyControl::handle(SDL_Event event)
{
    // save old position of mouse and get new
    this->_mousePositionXmem = this->_mousePositionX;
    this->_mousePositionYmem = this->_mousePositionY;
    Uint32 state = SDL_GetMouseState(&this->_mousePositionX, &this->_mousePositionY);

    // get info of pessed mouse button
    if (state & SDL_BUTTON(SDL_BUTTON_LEFT)) this->_leftClick = true;
    else this->_leftClick = false;

    if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) this->_rightClick = true;
    else this->_rightClick = false;

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
        auto it = std::find(this->_pressedKeyInFrame.begin(), this->_pressedKeyInFrame.end(), key);
        if (it == this->_pressedKeyInFrame.end()) { // bacase we want make this funcion only once in frame
            if (std::find(this->_lockedKey.begin(), this->_lockedKey.end(), key) == this->_lockedKey.end()) {
                auto function = this->_keyFunctions[key];
                if (function) function(); // make function only if function is not null
                this->_pressedKeyInFrame.push_back(key);
            }
        }
    }
}

void RustyControl::lockKey(SDL_Keycode key)
{
    this->_lockedKey.push_back(key);
}

void RustyControl::wipeLockedKey()
{
    this->_lockedKey.clear();
}

void RustyControl::reset()
{
    this->_mousePositionXmem = this->_mousePositionX;
    this->_mousePositionYmem = this->_mousePositionY;
    this->_pressedKeyInFrame.clear();
}

RRW_MouseInfo RustyControl::getMouseInfo()
{
    RRW_MouseInfo info;
    info.x = this->_mousePositionX;
    info.y = this->_mousePositionY;
    info.clickL = this->_leftClick;
    info.clickR = this->_rightClick;

    return info;
}

RRW_MouseMove RustyControl::getMouseMove()
{
    RRW_MouseMove move;
    move.vecx = this->_mousePositionX - this->_mousePositionXmem;
    move.vecy = this->_mousePositionY - this->_mousePositionYmem;

    return move;
}

bool RustyControl::_isPressed(SDL_Keycode key)
{
    auto it = std::find(this->_pressedKey.begin(), this->_pressedKey.end(), key);
    if (it != this->_pressedKey.end()) {
        return true;
    }
    return false;
}