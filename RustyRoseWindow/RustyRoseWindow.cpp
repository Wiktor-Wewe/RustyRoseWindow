#include <stdio.h>
#include "RustyRenderWindow.h"
#include "RustyControl.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int pressedYes() {
    printf("YES\n");
    return 1;
}

int pressedNo() {
    printf("NO\n");
    return 0;
}

int CloseWindow() {
    return -1;
}

void handleWindows(RustyWindowsManager* manager, RustyControl* control)
{
    if (manager->isAnyWindow()) {
        manager->getCurrentWindow()->updateSelectedId(control->getMouseInfo().x, control->getMouseInfo().y);

        if (control->getMouseInfo().clickL) {
            manager->updateCurrentWindow(control->getMouseInfo().x, control->getMouseInfo().y);
            if (checkMousePositionOnObject(control->getMouseInfo().x, control->getMouseInfo().y, manager->getCurrentWindow()->getBarPosition())) {
                auto move = control->getMouseMove();
                manager->getCurrentWindow()->move(move.vecx, move.vecy);
            }

            int response = manager->getCurrentWindow()->click();
            if (response == -1) {
                manager->removeCurrentWindow();
            }
        }
    }
}

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
    //SDL_Surface* sampleSurface = IMG_Load(pathToImg);
    //SDL_Texture* sampleTexture = SDL_CreateTextureFromSurface(renderer, sampleSurface);
    //SDL_FreeSurface(sampleSurface);

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

    RustyControl control;
    control.addKeyFunction(SDLK_w, NULL);
    control.addKeyFunction(SDLK_s, NULL);
    control.addKeyFunction(SDLK_a, NULL);
    control.addKeyFunction(SDLK_d, NULL);

    Fonts fonts(fontSmall, fontMedium, fontLarge);
    ScreenSize screenSize(1280, 720);

    RustyRenderWindow renderWindow(renderer, fonts.Medium, &screenSize);
    
    std::string text = "Are you sure you want to override save file?";
    RustyDialogWindow* dialogWindow = new RustyDialogWindow(text, renderer, &screenSize, fonts.Medium, fontSmall, 600, 400);
    dialogWindow->addText("Po nadpisaniu pliku nie bedzie juz mozlwiosci odwrotu wiec lepiej to wszystko przemysl.", 0, 0, fonts.Small);
    dialogWindow->centerTexts();
    dialogWindow->getButton(1)->setFunction(pressedYes);
    dialogWindow->getButton(2)->setFunction(pressedNo);
    dialogWindow->getButton(3)->setFunction(CloseWindow);
    dialogWindow->setPosition(50, 60);

    RustyWindow* rustyWindow = new RustyWindow(renderer, &screenSize, fonts.Medium, 600, 400);
    rustyWindow->addText("This is simple information :)", 200, 50, fonts.Large);
    rustyWindow->addButton("OK", 0, 0, 80, 30, fonts.Medium);
    rustyWindow->getButton(1)->setFunction(CloseWindow);
    rustyWindow->centerButtons();
    rustyWindow->centerTexts();

    renderWindow.getManager()->addWindow(rustyWindow);
    renderWindow.getManager()->addWindow(dialogWindow);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                control.handle(e);
            }
        }

        SDL_RenderClear(renderer);
        renderWindow.reversedDraw();
        SDL_RenderPresent(renderer);

        MouseInfo mouseInfo = control.getMouseInfo();
        MouseMove mouseMove = control.getMouseMove();
        renderWindow.getScene()->clear(RustyScene::Clear::Dialogs);
        renderWindow.getScene()->addDialog("Mouse x: " + std::to_string(mouseInfo.x));
        renderWindow.getScene()->addDialog("Mouse y: " + std::to_string(mouseInfo.y));
        renderWindow.getScene()->addDialog("Click Left: " + std::string(mouseInfo.clickL == true ? "True" : "False"));
        renderWindow.getScene()->addDialog("Click Right: " + std::string(mouseInfo.clickR == true ? "True" : "False"));
        renderWindow.getScene()->addDialog("Move X: " + std::to_string(mouseMove.vecx));
        renderWindow.getScene()->addDialog("Move Y: " + std::to_string(mouseMove.vecy));
        renderWindow.getScene()->addDialog("Current Window Id: " + std::to_string(renderWindow.getManager()->getCurrentWindowId()));

        handleWindows(renderWindow.getManager(), &control);
        
        control.resetMove();
        SDL_Delay(2);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}