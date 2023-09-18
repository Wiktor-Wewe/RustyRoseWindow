#include "RustyControl.h"

RustyControl::RustyControl()
{
	this->_status = false;
}

void RustyControl::add(Action x)
{
	this->_status = true;
	this->_pressedKeys.push_back(x);
}

void RustyControl::clear()
{
	this->_status = false;
	this->_pressedKeys.clear();
}

bool RustyControl::check(Action x)
{
	for (auto i : this->_pressedKeys) {
		if (i == x) {
			return true;
		}
	}
	return false;
}

bool RustyControl::isAction()
{
	return this->_status;
}

RustyControl::~RustyControl()
{
	this->_pressedKeys.clear();
}
