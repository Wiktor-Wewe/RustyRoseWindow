#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "RustyDialogWindow.h"
#include "RustyControl.h"
#include "RustyScene.h"
#include "RustyWindow.h"
#include "RustyWindowsManager.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RustyDialogWindow* dialogWindow;

void pressW() {
    dialogWindow->move(0, -1);
}

void pressS() {
    dialogWindow->move(0, 1);
}

void pressA() {
    dialogWindow->move(-1, 0);
}

void pressD() {
    dialogWindow->move(1, 0);
}

// todo
// poprawic move na pewno w miniWindow i chyba w dialogWindow
// to ze zacina sie gdy wyjade za mocno na boki
// gdy chce przesunac o 5 ale maksymalnie ekranu zostalo mi 4, to uciac 1 z 5 i przesunac
// dodac glowne okno z window managerem (RustyRenderWindow)
// zronic scene?
// zooptymalizowac mini window i dialog window
// dodac obsluge myszki

int main(int argc, char* args[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Can't initialize SDL! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("Can't initialize SDL_image! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() < 0) {
        printf("Can't initialize SDL_ttf! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "RustyRoseWindowTest",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,
        720,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        printf("Can't create window! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf("Can't create renderer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    const char* pathToImg = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseWindow\\x64\\Debug\\SAVE.PNG";
    SDL_Surface* sampleSurface = IMG_Load(pathToImg);
    SDL_Texture* sampleTexture = SDL_CreateTextureFromSurface(renderer, sampleSurface);

    const char fontPath[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseWindow\\x64\\Debug\\arial.ttf";
    TTF_Font* fontSmall = TTF_OpenFont(fontPath, 12);
    TTF_Font* fontMedium = TTF_OpenFont(fontPath, 24);
    TTF_Font* fontLarge = TTF_OpenFont(fontPath, 36);

    if (fontSmall == nullptr || fontMedium == nullptr || fontLarge == nullptr) {
        printf("Can't load font! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    Fonts fonts(fontSmall, fontMedium, fontLarge);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    ScreenSize screenSize(1280, 720);

    std::string text = "Are you sure you want to override save file?";
    dialogWindow = new RustyDialogWindow(text, renderer, &screenSize, fonts.Medium, fontSmall, 600, 400);

    int change = 0;
    int endMiniWindow = 0;
    bool hover = true;
    
    RustyScene scene(renderer, fonts.Large, &screenSize);
    RustyWindowsManager manager(renderer);
    manager.addWindow(dialogWindow);
    dialogWindow->setPosition(50, 60);

    RustyControl control;
    control.addKeyFunction(SDLK_w, pressW);
    control.addKeyFunction(SDLK_s, pressS);
    control.addKeyFunction(SDLK_a, pressA);
    control.addKeyFunction(SDLK_d, pressD);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

    RustyWindow* rustyWindow = new RustyWindow(renderer, &screenSize, fonts.Medium, 600, 400);
    rustyWindow->addText("wiktor", 200, 50, fonts.Large);
    manager.addWindow(rustyWindow);

    int dialogNumber = 0;
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                control.handle(e);
            }
        }

        SDL_RenderClear(renderer);
        manager.draw();
        scene.draw();
        SDL_RenderPresent(renderer);

        dialogWindow->setCursor(1);
        change++;
        if (change > 500) {
            if (hover) {
                hover = false;
                dialogWindow->moveCursor(-1);
            }
            else {
                hover = true;
                dialogWindow->moveCursor(1);
            }
            change = 0;
            scene.clear(RustyScene::Clear::Dialogs);
            scene.addDialog("Selected index: " + std::to_string(dialogWindow->getSelectedId()));
        }
        
        SDL_Delay(1);
    }

    delete dialogWindow;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}