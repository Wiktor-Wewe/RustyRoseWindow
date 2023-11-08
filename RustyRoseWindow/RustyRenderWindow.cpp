#include "RustyRenderWindow.h"

RustyRenderWindow::RustyRenderWindow(std::string windowName, int windowWidth, int windowHeight, const char* fontPath, int outlineSize)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        RRW_LogError("Can't initialize SDL! SDL_Error: \n" + std::string(SDL_GetError()));
        this->_initStatus = -1;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        RRW_LogError("Can't initialize SDL_image! SDL_image Error: \n" + std::string(IMG_GetError()));
        SDL_Quit();
        this->_initStatus = -2;
    }

    if (TTF_Init() < 0) {
        RRW_LogError("Can't initialize SDL_ttf! SDL_ttf Error: \n" + std::string(TTF_GetError()));
        SDL_Quit();
        this->_initStatus = -3;
    }

    this->_fonts = RRW_OpenFonts(fontPath, 12, 24, 36, outlineSize);
    
    if (this->_fonts == nullptr) {
        RRW_LogError("Can't load font! SDL_ttf Error: \n" + std::string(TTF_GetError()));
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
        RRW_LogError("Can't create window! SDL_Error: \n" + std::string(SDL_GetError()));
        IMG_Quit();
        SDL_Quit();
        this->_initStatus = -5;
    }

    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
    if (this->_renderer == nullptr) {
        RRW_LogError("Can't create renderer! SDL_Error: \n" + std::string(SDL_GetError()));
        SDL_DestroyWindow(this->_window);
        IMG_Quit();
        SDL_Quit();
        this->_initStatus = -6;
    }

    this->_initStatus = 0;
    RRW_Font* font = this->_fonts == nullptr ? nullptr : this->_fonts->mediumFont;
    this->_manager = new RustyWindowsManager(this->_renderer, this->_screenSize, font);
	this->_scene = new RustyScene(this->_renderer, font, this->_screenSize);

    SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);
}

void RustyRenderWindow::setFontSmall(const char* fontPath, int size, int outlineSize)
{
    if (this->_fonts->smallFont) {
        delete this->_fonts->smallFont;
    }
    this->_fonts->smallFont = new RRW_Font(fontPath, size, outlineSize);
}

void RustyRenderWindow::setFontMedium(const char* fontPath, int size, int outlineSize)
{
    if (this->_fonts->mediumFont) {
        delete this->_fonts->mediumFont;
    }
    this->_fonts->mediumFont = new RRW_Font(fontPath, size, outlineSize);
}

void RustyRenderWindow::setFontLarge(const char* fontPath, int size, int outlineSize)
{
    if (this->_fonts->largeFont) {
        delete this->_fonts->largeFont;
    }
    this->_fonts->largeFont = new RRW_Font(fontPath, size, outlineSize);
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
