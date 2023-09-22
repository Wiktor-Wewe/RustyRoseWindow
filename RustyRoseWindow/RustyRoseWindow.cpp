#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "RustyDialogWindow.h"
#include "RustyMiniWindow.h"
#include "RustyControl.h"
#include "RustyScene.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RustyMiniWindow* miniWindow;

void pressW() {
    miniWindow->move(0, -1);
}

void pressS() {
    miniWindow->move(0, 1);
}

void pressA() {
    miniWindow->move(-1, 0);
}

void pressD() {
    miniWindow->move(1, 0);
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
    RustyDialogWindow* dialogWindow = new RustyDialogWindow(text, 500, 230, fonts.Medium, fonts.Small, renderer, screenSize);
    dialogWindow->setTextFont(fonts.Large);
    dialogWindow->setButtonFont(fonts.Medium);
    dialogWindow->make();

    RustyButton button1(renderer, fonts.Medium, 1, "guzik numer 1", 200, 100, &screenSize);
    button1.setPosition(screenSize.Width - button1.getPosition()->w, 0);
    button1.make();

    RustyButton button2(renderer, fonts.Small, 1, "guzik numer 2", 200, 100, &screenSize);
    button2.setPosition(0, 0);
    button2.make();


    miniWindow = new RustyMiniWindow(800, 480, fonts.Medium, renderer, &screenSize);
    miniWindow->addText("Siema to test", 100, 100);
    miniWindow->addButton("Przycisk 1", 1, 80, 20, fonts.Small);
    miniWindow->getButton(1)->setHover(true);
    miniWindow->addButton("Przycisk 2", 2, 200, 50, fonts.Medium);
    miniWindow->getButton(2)->setHover(true);
    miniWindow->getButton(2)->setBackGroundColor({ 0xff, 0x00, 0xff, 0xff });
    miniWindow->getButton(2)->setPosition(20, 20);
    miniWindow->setCustomTexture(sampleTexture);
    miniWindow->setCustomTexture(NULL);
    miniWindow->getButton(2)->make();
    miniWindow->make();

    int change = 0;
    int endMiniWindow = 0;
    bool hover = true;
    
    RustyScene scene(renderer, fonts.Medium, &screenSize);

    RustyControl control;
    control.addKeyFunction(SDLK_w, pressW);
    control.addKeyFunction(SDLK_s, pressS);
    control.addKeyFunction(SDLK_a, pressA);
    control.addKeyFunction(SDLK_d, pressD);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

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
        miniWindow->draw();
        dialogWindow->draw();
        scene.draw();
        SDL_RenderPresent(renderer);

        
        change++;
        if (change > 500) {
            if (hover) {
                hover = false;
                dialogWindow->changeSelect(-1);
                scene.addDialog("Dialog: " + std::to_string(dialogNumber));
                scene.removeDialog("Dialog: " + std::to_string(dialogNumber - 1));
                dialogNumber++;
                if (dialogNumber % 3 == 0) {
                    scene.setFont(fonts.Small);
                }
                if (dialogNumber % 3 == 1) {
                    scene.setFont(fonts.Medium);
                }
                if (dialogNumber % 3 == 2) {
                    scene.setFont(fonts.Large);
                }
            }
            else {
                hover = true;
                dialogWindow->changeSelect(1);
            }
            change = 0;
        }

        miniWindow->getButton(1)->setHover(hover);
        miniWindow->getButton(2)->setHover(!hover);
        
        SDL_Delay(1);
    }

    delete miniWindow;
    delete dialogWindow;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}