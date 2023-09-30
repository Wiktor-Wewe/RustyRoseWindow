﻿#include <stdio.h>
#include <fstream>
#include "RustyRenderWindow.h"
#include "RustyControl.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RustyRenderWindow* renderWindow;
std::string utf8line;
int i = 0;

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

void makeNewWindow() {
    i++;
    std::string text = "Are you sure you want to override save file?";
    RustyDialogWindow* dialogWindow = new RustyDialogWindow(text, renderWindow->getRenderer(), renderWindow->getScreenSize(), renderWindow->getFonts()->medium, renderWindow->getFonts()->small, 600, 400);
    dialogWindow->addText("After overwriting the file, there will be no possibility to reverse this process, so it's better to carefully consider everything.", 0, 0, renderWindow->getFonts()->small, 350);
    dialogWindow->addText(utf8line, 0, 0, renderWindow->getFonts()->small, 350);
    dialogWindow->centerTexts();
    dialogWindow->getButton(1)->setFunction(pressedYes);
    dialogWindow->getButton(2)->setFunction(pressedNo);
    dialogWindow->getButton(3)->setFunction(CloseWindow);
    dialogWindow->setPosition(50+i, 60+i);

    renderWindow->getManager()->addWindow(dialogWindow);
}

void handleWindows(RustyWindowsManager* manager, RustyControl* control)
{
    if (manager->isAnyWindow()) {
        auto mouseInfo = control->getMouseInfo();
        manager->getCurrentWindow()->updateSelectedId(mouseInfo.x, mouseInfo.y);

        if (mouseInfo.clickL) {
            manager->updateCurrentWindow(mouseInfo.x, mouseInfo.y);
            if (RRW_CheckMousePositionOnObject(mouseInfo.x, mouseInfo.y, manager->getCurrentWindow()->getBarPosition())) {
                auto mouseMove = control->getMouseMove();
                manager->getCurrentWindow()->move(mouseMove.vecx, mouseMove.vecy);
            }

            int response = manager->getCurrentWindow()->click();
            if (response == -1) {
                manager->removeCurrentWindow();
            }
        }
    }
}

int main(int argc, char* args[]) {

    std::fstream textFile;
    const char textFilePath[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseWindow\\x64\\Debug\\utf8Test.txt";
    
    utf8line = "cant read text test file :c";
    textFile.open(textFilePath, std::ios::in);
    if (textFile.good()) {
        std::getline(textFile, utf8line);
        textFile.close();
    }
    
    const char fontPath[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseWindow\\x64\\Debug\\arial.ttf";

    renderWindow = new RustyRenderWindow("RustyWindowTest", SCREEN_WIDTH, SCREEN_HEIGHT, fontPath);
    printf("status: %i\n", renderWindow->getInitStatus());

    SDL_Renderer* renderer = renderWindow->getRenderer();
    RRW_Fonts* fonts = renderWindow->getFonts();
    RRW_ScreenSize* screenSize = renderWindow->getScreenSize();

    RustyControl control;
    control.addKeyFunction(SDLK_SPACE, makeNewWindow);

    std::string text = "Are you sure you want to override save file?";
    RustyDialogWindow* dialogWindow = new RustyDialogWindow(text, renderer, screenSize, fonts->medium, fonts->small, 600, 400);
    dialogWindow->setBackgroundColor({ 0, 0, 0, 200 });
    dialogWindow->addText("After overwriting the file, there will be no possibility to reverse this process, so it's better to carefully consider everything.", 0, 0, fonts->small, 350);
    dialogWindow->addText(utf8line, 0, 0, fonts->small, 350);
    dialogWindow->centerTexts();
    dialogWindow->getButton(1)->setFunction(pressedYes);
    dialogWindow->getButton(2)->setFunction(pressedNo);
    dialogWindow->getButton(3)->setFunction(CloseWindow);
    dialogWindow->setPosition(50, 60);

    RustyWindow* rustyWindow = new RustyWindow(renderer, screenSize, fonts->medium, 600, 400);
    rustyWindow->addText("This is simple information :)", 200, 50, fonts->large);
    rustyWindow->addButton("OK", 0, 0, 80, 30, fonts->medium);
    rustyWindow->getButton(1)->setFunction(CloseWindow);
    rustyWindow->centerButtons();
    rustyWindow->centerTexts();

    renderWindow->getManager()->addWindow(rustyWindow);
    renderWindow->getManager()->addWindow(dialogWindow);

    std::string statusText = "RustyRenderWindow init status: " + std::to_string(renderWindow->getInitStatus());
    renderWindow->getScene()->addText(statusText, 0, 0, { 0xff, 0xff, 0xff, 0xff }, { 0xff, 0x00, 0x00, 0xff }, fonts->medium);

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

        renderWindow->reversedDraw();

        RRW_MouseInfo mouseInfo = control.getMouseInfo();
        RRW_MouseMove mouseMove = control.getMouseMove();
        renderWindow->getScene()->clear(RustyScene::Clear::Dialogs);
        renderWindow->getScene()->addDialog("Mouse x: " + std::to_string(mouseInfo.x));
        renderWindow->getScene()->addDialog("Mouse y: " + std::to_string(mouseInfo.y));
        renderWindow->getScene()->addDialog("Click Left: " + std::string(mouseInfo.clickL == true ? "True" : "False"));
        renderWindow->getScene()->addDialog("Click Right: " + std::string(mouseInfo.clickR == true ? "True" : "False"));
        renderWindow->getScene()->addDialog("Move X: " + std::to_string(mouseMove.vecx));
        renderWindow->getScene()->addDialog("Move Y: " + std::to_string(mouseMove.vecy));
        renderWindow->getScene()->addDialog("Current Window Id: " + std::to_string(renderWindow->getManager()->getCurrentWindowId()));

        handleWindows(renderWindow->getManager(), &control);
        
        control.resetMove();
        SDL_Delay(2);
    }

    delete renderWindow;
    return 0;
}