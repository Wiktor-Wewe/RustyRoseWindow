#pragma once
#include "SharedResourceUtil.h"
#include "RustyWindowsManager.h"
#include "RustyScene.h"

class RustyRenderWindow
{
public:
	RustyRenderWindow();


	~RustyRenderWindow();

private:
	RustyWindowsManager* _manager;
	RustyScene* _scene;

};

