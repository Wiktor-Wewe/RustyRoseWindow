#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "RustyDialogWindow.h"
#include "RustyMiniWindow.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
    RustyDialogWindow dialogWindow(text, 500, 230, fonts.Medium, fonts.Small, renderer, screenSize);
    dialogWindow.setTextFont(fonts.Large);
    dialogWindow.setButtonFont(fonts.Medium);
    dialogWindow.make();

    RustyButton button1(renderer, fonts.Medium, 1, "guzik numer 1", 200, 100, &screenSize);
    button1.setPosition(screenSize.Width - button1.getPosition()->w, 0);
    button1.make();

    RustyButton button2(renderer, fonts.Small, 1, "guzik numer 2", 200, 100, &screenSize);
    button2.setPosition(0, 0);
    button2.make();


    std::string repsponse = dialogWindow.show() ? "True" : "False";
    SDL_Rect* responseSize = new SDL_Rect();
    SDL_Texture* reposneTexture = makeTextureFromText(repsponse, responseSize, fonts.Large, { 0xff, 0xff, 0xff, 0xff }, { 0xff, 0x00, 0xff, 0xff }, renderer, screenSize.Width);
    responseSize->x = (screenSize.Width / 2) - (responseSize->w / 2);
    responseSize->y = (screenSize.Height / 2) - (responseSize->h / 2);

    int change = 0;
    bool hover = true;

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, reposneTexture, NULL, responseSize);
        SDL_RenderCopy(renderer, button1.getTexture(), NULL, button1.getPosition());
        SDL_RenderCopy(renderer, button2.getTexture(), NULL, button2.getPosition());
        SDL_RenderPresent(renderer);
        
        change++;
        if (change > 500) {
            if (hover) {
                hover = false;
            }
            else {
                hover = true;
            }
            change = 0;
        }

        button1.setHover(hover);
        button2.setHover(!hover);

    }

    SDL_DestroyTexture(reposneTexture);
    delete responseSize;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}