#include "RustyMiniWindow.h"

RustyMiniWindow::RustyMiniWindow(int width, int height, TTF_Font* defaultFont, SDL_Renderer* renderer, ScreenSize* screenSize)
{
	this->_selectedId = 0;
	this->_position = new SDL_Rect;
	this->_position->w = width;
	this->_position->h = height;
	this->_renderer = renderer;
	this->_defaultFont = defaultFont;
	this->_screenSize = screenSize;

	this->_position->x = (this->_screenSize->Width / 2) - (this->_position->w / 2);
	this->_position->y = (this->_screenSize->Height / 2) - (this->_position->h / 2);

	this->_backGroundColor = { 0x00, 0x00, 0x00, 0xff };
	this->_windowHoverColor = { 0xff, 0x99, 0x00, 0xff };
	this->_fontColor = { 0xff, 0xff, 0xff, 0xff };
	this->_fontHoverColor = { 0xff, 0x99, 0x00, 0xff };

	this->_texture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->_position->w, this->_position->h);
	SDL_SetTextureBlendMode(this->_texture, SDL_BLENDMODE_BLEND);
}

void RustyMiniWindow::setPosition(int x, int y)
{
	this->_position->x = x;
	this->_position->y = y;
}

void RustyMiniWindow::setSize(int width, int height)
{
	this->_position->w = width;
	this->_position->h = height;
}

void RustyMiniWindow::setDefaultFont(TTF_Font* font)
{
	this->_defaultFont = font;
}

void RustyMiniWindow::setBackGroundColor(SDL_Color color)
{
	this->_backGroundColor = color;
}

void RustyMiniWindow::setWindowHoverColor(SDL_Color color)
{
	this->_windowHoverColor = color;
}

void RustyMiniWindow::setFontColor(SDL_Color color)
{
	this->_fontColor = color;
}

void RustyMiniWindow::setFontHoverColor(SDL_Color color)
{
	this->_fontHoverColor = color;
}

void RustyMiniWindow::addText(std::string text, int x, int y, TTF_Font* font)
{
	WindowText* windowText = new WindowText;
	windowText->text = text;
	windowText->rect = new SDL_Rect;
	windowText->rect->x = x;
	windowText->rect->y = y;
	auto windowFont = font == NULL ? this->_defaultFont : font;
	windowText->texture = makeTextureFromText(text, windowText->rect, windowFont, this->_fontColor, this->_fontHoverColor, this->_renderer, this->_position->w);

	this->_text.push_back(windowText);
}

void RustyMiniWindow::removeText(std::string text)
{
	for (int i = 0; i < this->_text.size(); i++) {
		if (this->_text[i]->text == text) {
			delete this->_text[i];
			this->_text.erase(this->_text.begin() + i);
			return;
		}
	}
}

void RustyMiniWindow::addButton(std::string text, int id, int width, int height, TTF_Font* font)
{
	TTF_Font* buttonFont = font == NULL ? this->_defaultFont : font;
	RustyButton* button = new RustyButton(this->_renderer, buttonFont, id, text, width, height, this->_screenSize);
	button->make();
	this->_buttons.push_back(button);
}

void RustyMiniWindow::removeButton(int id)
{
	for (int i = 0; i < this->_buttons.size(); i++) {
		if (this->_buttons[i]->getId() == id) {
			delete this->_buttons[i];
			this->_buttons.erase(this->_buttons.begin() + i);
			return;
		}
	}
}

RustyButton* RustyMiniWindow::getButton(int id)
{
	for (auto button : this->_buttons) {
		if (button->getId() == id) {
			return button;
		}
	}
	return nullptr;
}

void RustyMiniWindow::selectId(int id)
{
	this->_selectedId = id;
}

void RustyMiniWindow::move(int vecx, int vecy)
{
	if (this->_position->x + this->_position->w + vecx < this->_screenSize->Width && this->_position->x > 0) {
		this->_position->x += vecx;
	}

	if (this->_position->y + this->_position->h + vecy < this->_screenSize->Height && this->_position->y > 0) {
		this->_position->y += vecy;
	}
}

void RustyMiniWindow::make()
{
	// get old parameters
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	// make window background
	SDL_SetRenderDrawColor(this->_renderer, this->_backGroundColor.r, this->_backGroundColor.g, this->_backGroundColor.b, this->_backGroundColor.a);
	SDL_SetRenderTarget(this->_renderer, this->_texture);
	SDL_RenderClear(this->_renderer);
	
	// make window hover
	SDL_SetRenderDrawColor(this->_renderer, this->_windowHoverColor.r, this->_windowHoverColor.g, this->_windowHoverColor.b, this->_windowHoverColor.a);
	SDL_Rect hoverRect { 0, 0, this->_position->w, this->_position->h };
	SDL_RenderDrawRect(this->_renderer, &hoverRect);

	// draw text on texture
	for (auto text : this->_text) {
		SDL_RenderCopy(this->_renderer, text->texture, NULL, text->rect);
	}

	// set old parameters
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

void RustyMiniWindow::draw()
{
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	SDL_SetRenderTarget(this->_renderer, NULL);
	SDL_RenderCopy(this->_renderer, this->_texture, NULL, this->_position);
	for (auto button : this->_buttons) {
		auto p = button->getPosition();
		SDL_Rect rect{ p->x + this->_position->x, p->y + this->_position->y, p->w, p->h };
		SDL_RenderCopy(this->_renderer, button->getTexture(), NULL, &rect);
	}

	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

RustyMiniWindow::~RustyMiniWindow()
{
	delete this->_position;
	
	for (auto text : this->_text) {
		delete text;
	}

	for (auto button : this->_buttons) {
		delete button;
	}

	SDL_DestroyTexture(this->_texture);
}