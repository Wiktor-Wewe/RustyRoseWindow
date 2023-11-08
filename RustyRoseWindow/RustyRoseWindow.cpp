#include <fstream>
#include "RustyRenderWindow.h"
#include "RustyControl.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RustyRenderWindow* renderWindow;
std::string utf8line;
int i = 0;

int pressedYes() {
    RRW_LogInfo("YES\n");
    return 1;
}

int pressedOk() {
    RRW_LogError("ok.");
    return 1;
}

int pressedNo() {
    RRW_LogInfo("NO\n");
    return 0;
}

int closeWindow() {
    return -1;
}

void pressC() {
    RRW_LogInfo("c was pressed\n");
}

void hide() {
    renderWindow->getManager()->getCurrentWindow()->hide();
}

void show() {
    renderWindow->getManager()->getCurrentWindow()->show();
}

void makeNewWindow() {
    i++;
    std::string text = "Are you sure you want to override save file?";
    RustyWindow* dialogWindow = new RustyWindow(renderWindow->getRenderer(), renderWindow->getScreenSize(), nullptr, 400, 300);
    renderWindow->getManager()->addWindow(dialogWindow);
    dialogWindow->addText("After overwriting the file, there will be no possibility to reverse this process, so it's better to carefully consider everything.", 0, 0, renderWindow->getFonts()->smallFont, 350);
    dialogWindow->addText(utf8line, 0, 0, renderWindow->getFonts()->smallFont, 350);
    dialogWindow->centerTexts();
    dialogWindow->setPosition(50+i, 60+i);
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

            auto currentWindow = manager->getCurrentWindow();
            if (currentWindow) {
                int response = currentWindow->click();
                if (response == -1) {
                    manager->removeCurrentWindow();
                }
            }
        }
    }
}

int main(int argc, char* args[]) {
    RRW_OpenConsole();
    std::fstream textFile;
    const char textFilePath[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseWindow\\x64\\Debug\\utf8Test.txt";
    
    utf8line = "cant read text test file :c";
    textFile.open(textFilePath, std::ios::in);
    if (textFile.good()) {
        std::getline(textFile, utf8line);
        textFile.close();
    }

    const char fontPath[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseWindow\\x64\\Debug\\arial.ttf";

    renderWindow = new RustyRenderWindow("RustyWindowTest", SCREEN_WIDTH, SCREEN_HEIGHT, fontPath, 1);
    RRW_LogInfo("status: " + std::to_string(renderWindow->getInitStatus()));

    SDL_Renderer* renderer = renderWindow->getRenderer();
    RRW_Fonts* fonts = renderWindow->getFonts();
    RRW_ScreenSize* screenSize = renderWindow->getScreenSize();

    RustyControl control;
    control.addKeyFunction(SDLK_SPACE, makeNewWindow);
    control.addKeyFunction(SDLK_c, pressC);
    control.addKeyFunction(SDLK_h, hide);
    control.addKeyFunction(SDLK_s, show);

    std::string text = "Are you sure you want to override save file?";

    RustyWindow* dialogWindow = new RustyWindow(renderer, screenSize, nullptr, 400, 100);
    renderWindow->getManager()->addWindow(dialogWindow);
    dialogWindow->setBackgroundColor({ 0, 0, 0, 200 });
    dialogWindow->addText("After overwriting the file, there will be no possibility to reverse this process, so it's better to carefully consider everything.", 0, 0, fonts->smallFont, 350);
    dialogWindow->addText(utf8line, 0, 0, fonts->smallFont, 350);
    dialogWindow->centerTexts();
    dialogWindow->setPosition(50, 60);

    RustyWindow* rustyWindow = new RustyWindow(renderer, screenSize, fonts->mediumFont, 600, 400);
    renderWindow->getManager()->addWindow(rustyWindow);
    rustyWindow->addText("This is simple information :)This is simple information :)This is simple information :)This is simple information :)This is simple information :)This is simple information :)This is simple information :)This is simple information :)This is simple information :)This is simple information :)", 200, 50, fonts->smallFont);
    rustyWindow->addButton("OK", 0, 0, 80, 30, fonts->mediumFont);
    rustyWindow->getButton(1)->setFunction(closeWindow);
    rustyWindow->getButton(1)->setBackgroundColor({ 0xff, 0x00, 0x00, 0xff });
    rustyWindow->centerButtons();
    rustyWindow->centerTexts();

    std::string statusText = "RustyRenderWindow init status: " + std::to_string(renderWindow->getInitStatus());
    renderWindow->getScene()->addText(statusText, 0, 0, { 0xff, 0xff, 0xff, 0xff }, { 0xff, 0x00, 0x00, 0xff }, fonts->mediumFont);


    auto testWindow = new RustyWindow(renderer, screenSize, nullptr, 400, 300);
    renderWindow->getManager()->addWindow(testWindow);
    testWindow->addText("Ori <3", 0, 0);
    testWindow->centerTexts();
    testWindow->addButton("OK", 0, 0, 0, 0);
    testWindow->addButton("NO", 0, 0, 0, 0);
    testWindow->addButton("DUPKA", 0, 0, 0, 0);
    testWindow->formatButtons();
    testWindow->formatWindow();
    testWindow->centerWindow();
    testWindow->addCloseButton();

    int newWindowId;
    auto newWindow = renderWindow->getManager()->makeWindow(300, 150, newWindowId);
    newWindow->addCloseButton();
    newWindow->addText("Sciana to swietny kolega!", 0, 0, nullptr);
    newWindow->hideBar();

    auto kacperWindow = renderWindow->getManager()->makeWindow(200, 80, newWindowId);
    kacperWindow->addCloseButton();
    kacperWindow->addText("Wiktor... Po pierwsze to nie jestesmy kolegami... po drugie...", 0, 0, fonts->smallFont);
    kacperWindow->addButton("ok", 0, 0, 80, 20, fonts->smallFont);
    kacperWindow->getButton(2)->setFunction(pressedOk);
    kacperWindow->centerButtons();

    kacperWindow->centerTexts();

    control.lockKey(SDLK_c);

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
        
        control.reset();
        SDL_Delay(2);
    }

    delete renderWindow;
    RRW_CloseConsole();
    return 0;
}