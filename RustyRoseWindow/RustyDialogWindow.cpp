#include "RustyDialogWindow.h"

RustyDialogWindow::RustyDialogWindow(std::string text, int windowWidth, int windowHeight, TTF_Font* textFont, TTF_Font* buttonFont, SDL_Renderer* renderer, ScreenSize screenSize)
{
	this->_text = text;

	this->_position.w = windowWidth;
	this->_position.h = windowHeight;
	this->_position.x = (screenSize.Width / 2) - (windowWidth / 2);
	this->_position.y = (screenSize.Height / 2) - (windowHeight / 2);

	this->_renderer = renderer;
	
	this->_selectIndex = 0;
	this->_end = false;
	this->_backGroundColor = { 0x00, 0x00, 0x00, 0xff };
	this->_hoverColor = { 0xff, 0x99, 0x00, 0xff };
	this->_fontColor = { 0xff, 0xff, 0xff, 0xff };
	this->_selectColor = { 0xff ,0x00, 0x00, 0xff };

	this->textFont = textFont;
	this->buttonFont = buttonFont;

	this->_positionButton1.w = windowWidth / 3;
	this->_positionButton1.h = windowHeight / 4;
	this->_positionButton1.x = (windowWidth / 4) - (this->_positionButton1.w / 2);
	this->_positionButton1.y = (this->_positionButton1.h * 3) - 5;

	this->_positionButton2.w = windowWidth / 3;
	this->_positionButton2.h = windowHeight / 4;
	this->_positionButton2.x = ((windowWidth / 4) * 3) - (this->_positionButton2.w / 2);
	this->_positionButton2.y = (this->_positionButton1.h * 3) - 5;

	this->_mainTexture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->_position.w, this->_position.h);
	this->_mainTextureHover = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->_position.w, this->_position.h);
	SDL_SetTextureBlendMode(this->_mainTextureHover, SDL_BLENDMODE_BLEND);
}

void RustyDialogWindow::setBackgroundColor(SDL_Color color)
{
	this->_backGroundColor = color;
}

void RustyDialogWindow::setHoverColor(SDL_Color color)
{
	this->_hoverColor = color;
}

void RustyDialogWindow::setFontColor(SDL_Color color)
{
	this->_fontColor = color;
}

void RustyDialogWindow::setSelectColor(SDL_Color color)
{
	this->_selectColor = color;
}

void RustyDialogWindow::setTextFont(TTF_Font* font)
{
	this->textFont = font;
}

void RustyDialogWindow::setButtonFont(TTF_Font* font)
{
	this->buttonFont = font;
}

void RustyDialogWindow::setCustomTexture(SDL_Texture* texture)
{
	this->_texture = texture;
}

void RustyDialogWindow::setCustomHoverTexture(SDL_Texture* texture)
{
	this->_textureHover = texture;
}

void RustyDialogWindow::setCustomPositionOfButtons(SDL_Rect button1, SDL_Rect button2)
{
	this->_positionButton1 = button1;
	this->_positionButton2 = button2;
}

void RustyDialogWindow::move(int vecx, int vecy)
{
	if (this->_position.x + this->_position.w + vecx < 1280 && this->_position.x > 0) {
		this->_position.x += vecx;
	}

	if (this->_position.y + this->_position.h + vecy < 720 && this->_position.y > 0) {
		this->_position.y += vecy;
	}
}

void RustyDialogWindow::changeSelect(int vecx)
{
	if (vecx > 0) {
		this->_selectIndex = 2;
	}
	else if (vecx < 0) {
		this->_selectIndex = 1;
	}
	this->_updateHoverTexture();
}

int RustyDialogWindow::enter()
{
	return this->_selectIndex;
}

void RustyDialogWindow::make()
{
	// get old parameters
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	// draw background of window
	SDL_SetRenderDrawColor(this->_renderer, this->_backGroundColor.r, this->_backGroundColor.g, this->_backGroundColor.b, this->_backGroundColor.a);
	SDL_SetRenderTarget(this->_renderer, this->_mainTexture);
	SDL_RenderClear(this->_renderer);
	SDL_SetRenderDrawColor(this->_renderer, this->_hoverColor.r, this->_hoverColor.g, this->_hoverColor.b, this->_hoverColor.a);

	// draw frame of window
	SDL_Rect framePosition = { 0, 0, this->_position.w, this->_position.h };
	SDL_RenderDrawRect(this->_renderer, &framePosition);
	
	// draw main text
	SDL_Rect* textTextureSize = new SDL_Rect();
	SDL_Texture* textTexture = makeTextureFromText(this->_text, textTextureSize, this->textFont, this->_fontColor, this->_hoverColor, this->_renderer, this->_position.w - 10);
	textTextureSize->x = (this->_position.w / 2) - (textTextureSize->w / 2) + 5;
	textTextureSize->y = 5;
	SDL_RenderCopy(this->_renderer, textTexture, NULL, textTextureSize);

	// fraw frames of buttons
	SDL_RenderDrawRect(this->_renderer, &this->_positionButton1);
	SDL_RenderDrawRect(this->_renderer, &this->_positionButton2);

	// draw yes button
	SDL_Rect* yesTextTextureSize = new SDL_Rect();
	SDL_Texture* yesTextTexture = makeTextureFromText("Yes", yesTextTextureSize, this->buttonFont, this->_fontColor, this->_hoverColor, this->_renderer, this->_positionButton1.w - 10);
	yesTextTextureSize->x = (this->_positionButton1.w / 2) - (yesTextTextureSize->w / 2) + this->_positionButton1.x;
	yesTextTextureSize->y = this->_positionButton1.y + 5;
	SDL_RenderCopy(this->_renderer, yesTextTexture, NULL, yesTextTextureSize);

	// draw no button
	SDL_Rect* noTextTextureSize = new SDL_Rect();
	SDL_Texture* noTextTexture = makeTextureFromText("No", noTextTextureSize, this->buttonFont, this->_fontColor, this->_hoverColor, this->_renderer, this->_positionButton2.w - 10);
	noTextTextureSize->x = (this->_positionButton2.w / 2) - (noTextTextureSize->w / 2) + this->_positionButton2.x;
	noTextTextureSize->y = this->_positionButton2.y + 5;
	SDL_RenderCopy(this->_renderer, noTextTexture, NULL, noTextTextureSize);

	// destory leftovers
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(yesTextTexture);
	SDL_DestroyTexture(noTextTexture);
	delete textTextureSize;
	delete yesTextTextureSize;
	delete noTextTextureSize;

	SDL_SetRenderTarget(this->_renderer, this->_mainTextureHover);
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
	SDL_RenderClear(this->_renderer);
	
	// set old parameters
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

void RustyDialogWindow::draw()
{
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	SDL_SetRenderTarget(this->_renderer, NULL);
	SDL_RenderCopy(this->_renderer, this->_mainTexture, NULL, &this->_position);
	SDL_RenderCopy(this->_renderer, this->_mainTextureHover, NULL, &this->_position);

	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

RustyDialogWindow::~RustyDialogWindow()
{
	SDL_DestroyTexture(this->_mainTexture);
	SDL_DestroyTexture(this->_mainTextureHover);
	SDL_DestroyTexture(this->_texture);
	SDL_DestroyTexture(this->_textureHover);
}

void RustyDialogWindow::_updateHoverTexture()
{
	// get old parameters
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	// do stuff
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
	SDL_SetRenderTarget(this->_renderer, this->_mainTextureHover);
	SDL_RenderClear(this->_renderer);

	SDL_SetRenderDrawColor(this->_renderer, this->_selectColor.r, this->_selectColor.g, this->_selectColor.b, this->_selectColor.a);

	if (this->_selectIndex == 1) {
		SDL_RenderDrawRect(this->_renderer, &this->_positionButton1);
	}
	else if (this->_selectIndex == 2) {
		SDL_RenderDrawRect(this->_renderer, &this->_positionButton2);
	}

	// set old parameters
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	SDL_SetRenderTarget(this->_renderer, oldTarget);
}
