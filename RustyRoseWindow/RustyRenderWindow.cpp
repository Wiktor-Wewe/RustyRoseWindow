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

    TTF_Font* fontSmall = TTF_OpenFont(fontPath, 12);
    TTF_Font* fontMedium = TTF_OpenFont(fontPath, 24);
    TTF_Font* fontLarge = TTF_OpenFont(fontPath, 36);

    if (fontSmall == nullptr || fontMedium == nullptr || fontLarge == nullptr) {
        printf("Can't load font! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        this->_initStatus = -4;
    }

    this->_fonts = new Fonts(fontSmall, fontMedium, fontLarge);

    this->_screenSize = new ScreenSize(windowWidth, windowHeight);

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
	this->_scene = new RustyScene(this->_renderer, this->_fonts->Medium, this->_screenSize);
}

void RustyRenderWindow::setFontSmall(const char* fontPath, int size)
{
    if (this->_fonts->Small) {
        TTF_CloseFont(this->_fonts->Small);
    }
    this->_fonts->Small = TTF_OpenFont(fontPath, size);
}

void RustyRenderWindow::setFontMedium(const char* fontPath, int size)
{
    if (this->_fonts->Medium) {
        TTF_CloseFont(this->_fonts->Medium);
    }
    this->_fonts->Medium = TTF_OpenFont(fontPath, size);
}

void RustyRenderWindow::setFontLarge(const char* fontPath, int size)
{
    if (this->_fonts->Large) {
        TTF_CloseFont(this->_fonts->Large);
    }
    this->_fonts->Large = TTF_OpenFont(fontPath, size);
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

ScreenSize* RustyRenderWindow::getScreenSize()
{
    return this->_screenSize;
}

Fonts* RustyRenderWindow::getFonts()
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
