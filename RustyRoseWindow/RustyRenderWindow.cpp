#include "RustyRenderWindow.h"

RustyRenderWindow::RustyRenderWindow(std::string windowName, int windowWidth, int windowHeight, const char* fontPath)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Can't initialize SDL! SDL_Error: %s\n", SDL_GetError());
        this->_initStatus = -1;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("Can't initialize SDL_image! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        this->_initStatus = -2;
    }

    if (TTF_Init() < 0) {
        printf("Can't initialize SDL_ttf! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
        this->_initStatus = -3;
    }

    this->_fonts = RRW_OpenFonts(fontPath, 12, 24, 36, 1);
    
    if (this->_fonts == nullptr) {
        printf("Can't load font! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        this->_initStatus = -4;
    }

    this->_screenSize = new RRW_ScreenSize(windowWidth, windowHeight);

    this->_window = SDL_CreateWindow(
        windowName.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        this->_screenSize->Width,
        this->_screenSize->Height,
        SDL_WINDOW_SHOWN
    );

    if (this->_window == nullptr) {
        printf("Can't create window! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        this->_initStatus = -5;
    }

    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
    if (this->_renderer == nullptr) {
        printf("Can't create renderer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(this->_window);
        IMG_Quit();
        SDL_Quit();
        this->_initStatus = -6;
    }

    this->_initStatus = 0;
    this->_manager = new RustyWindowsManager();
	this->_scene = new RustyScene(this->_renderer, this->_fonts->medium, this->_screenSize);

    SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);
}

void RustyRenderWindow::setFontSmall(const char* fontPath, int size, int outlineSize)
{
    if (this->_fonts->small) {
        delete this->_fonts->small;
    }
    this->_fonts->small = new RRW_Font(fontPath, size, outlineSize);
}

void RustyRenderWindow::setFontMedium(const char* fontPath, int size, int outlineSize)
{
    if (this->_fonts->medium) {
        delete this->_fonts->medium;
    }
    this->_fonts->medium = new RRW_Font(fontPath, size, outlineSize);
}

void RustyRenderWindow::setFontLarge(const char* fontPath, int size, int outlineSize)
{
    if (this->_fonts->large) {
        delete this->_fonts->large;
    }
    this->_fonts->large = new RRW_Font(fontPath, size, outlineSize);
}

RustyWindowsManager* RustyRenderWindow::getManager()
{
	return this->_manager;
}

RustyScene* RustyRenderWindow::getScene()
{
	return this->_scene;
}

int RustyRenderWindow::getInitStatus()
{
    return this->_initStatus;
}

SDL_Window* RustyRenderWindow::getWindow()
{
    return this->_window;
}

SDL_Renderer* RustyRenderWindow::getRenderer()
{
    return this->_renderer;
}

RRW_ScreenSize* RustyRenderWindow::getScreenSize()
{
    return this->_screenSize;
}

RRW_Fonts* RustyRenderWindow::getFonts()
{
    return this->_fonts;
}

void RustyRenderWindow::draw()
{
    SDL_RenderClear(this->_renderer);
	this->_scene->draw();
	this->_manager->draw();
    SDL_RenderPresent(this->_renderer);
}

void RustyRenderWindow::reversedDraw()
{
    SDL_RenderClear(this->_renderer);
	this->_manager->draw();
	this->_scene->draw();
    SDL_RenderPresent(this->_renderer);
}

RustyRenderWindow::~RustyRenderWindow()
{
	delete this->_manager;
	delete this->_scene;
    delete this->_screenSize;
    delete this->_fonts;

    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
