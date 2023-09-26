#include "RustyWindow.h"

RustyWindow::RustyWindow(SDL_Renderer* renderer, ScreenSize* screenSize, TTF_Font* font, int width, int height)
{
	this->_buttonIdCounter = 1;
	this->_renderer = renderer;
	this->_screenSize = screenSize;
	this->_font = font;
	this->_position = new SDL_Rect;
	this->_position->w = width;
	this->_position->h = height;
	this->_customTexture = NULL;
	
	this->_position->x = (this->_screenSize->Width / 2) - (this->_position->w / 2);
	this->_position->y = (this->_screenSize->Height / 2) - (this->_position->h / 2);

	this->_selectedId = -1;
	this->_backgroundColor = { 0x00, 0x00, 0x00, 0xff };
	this->_borderColor = { 0xff, 0x99, 0x00, 0xff };
	this->_fontColor = { 0xff, 0xff, 0xff,0xff };
	this->_fontOutlineColor = { 0xff, 0x99, 0x00, 0xff };
	this->_selectHoverColor = { 0xff, 0x00, 0x00, 0xff };
}

void RustyWindow::setFont(TTF_Font* font)
{
	this->_font = font;
}

void RustyWindow::setPosition(int x, int y)
{
	this->_position->x = x;
	this->_position->y = y;
}

void RustyWindow::setSize(int width, int height)
{
	if(width > 0)
		this->_position->w = width;

	if(height > 0)
		this->_position->h = height;
}

void RustyWindow::setBackgroundColor(SDL_Color color)
{
	this->_backgroundColor = color;
}

void RustyWindow::setBorderColor(SDL_Color color)
{
	this->_borderColor = color;
}

void RustyWindow::setFontColor(SDL_Color color)
{
	this->_fontColor = color;
}

void RustyWindow::setFontOutlineColor(SDL_Color color)
{
	this->_fontOutlineColor = color;
}

void RustyWindow::setSelectHoverColor(SDL_Color color)
{
	this->_selectHoverColor = color;
}

void RustyWindow::setCustomTexture(SDL_Texture* texture)
{
	this->_customTexture = texture;
}

void RustyWindow::addText(std::string text, int x, int y, TTF_Font* font)
{
	ImageText* imageText = new ImageText;
	imageText->text = text;
	imageText->rect->x = x;
	imageText->rect->y = y;
	auto windowFont = font == NULL ? this->_font : font;
	imageText->texture = makeTextureFromText(text, imageText->rect, windowFont, this->_fontColor, this->_fontOutlineColor, this->_renderer, this->_position->w);

	this->_texts.push_back(imageText);
}

void RustyWindow::removeText(std::string text)
{
	for (int i = 0; i < this->_texts.size(); i++) {
		if (this->_texts[i]->text == text) {
			delete this->_texts[i];
			this->_texts.erase(this->_texts.begin() + i);
			return;
		}
	}
}

void RustyWindow::centerTexts()
{
	int partOfScreenY = (this->_position->h * 0.83) / (this->_texts.size() + 1);

	for (int i = 0; i < this->_texts.size(); i++) {
		this->_texts[i]->rect->x = (this->_position->w / 2) - (this->_texts[i]->rect->w / 2);
		this->_texts[i]->rect->y = (partOfScreenY + (i * partOfScreenY)) - (this->_texts[i]->rect->h / 2);
	}
}

unsigned int RustyWindow::addButton(std::string text, int x, int y, int width, int height, TTF_Font* font)
{
	// set font and id
	TTF_Font* buttonFont = font == NULL ? this->_font : font;
	unsigned int id = this->_buttonIdCounter;

	// add button and text on it
	RustyButton* button = new RustyButton(id, this->_renderer, this->_screenSize, font, x, y, width, height);
	button->setText(text);
	this->_buttons.push_back(button);

	// move counter and return id of new button
	this->_buttonIdCounter++;
	return id;
}

void RustyWindow::removeButton(int id)
{
	for (int i = 0; i < this->_buttons.size(); i++) {
		if (this->_buttons[i]->getId() == id) {
			delete this->_buttons[i];
			this->_buttons.erase(this->_buttons.begin() + i);
			return;
		}
	}
}

void RustyWindow::centerButtons()
{
	int partOfScreenX = this->_position->w / (this->_buttons.size() + 1);
	int partOfScreenY = this->_position->h / 6;

	for (int i = 0; i < this->_buttons.size(); i++) {
		int tempX = (partOfScreenX + (partOfScreenX * i)) - ((this->_buttons[i]->getPosition()->w) / 2);
		this->_buttons[i]->setPosition(tempX, partOfScreenY * 5);
	}
}

RustyButton* RustyWindow::getButton(int id)
{
	for (auto button : this->_buttons) {
		if (button->getId() == id) {
			return button;
		}
	}

	return nullptr;
}

int RustyWindow::getSelectedId()
{
	return this->_selectedId;
}

void RustyWindow::moveCursor(int direction)
{
	for (auto button : this->_buttons) {
		button->setSelect(false);
	}

	if (this->_selectedId + direction > 0 && this->_selectedId + direction < this->_buttonIdCounter) {
		this->_selectedId += direction;
	}

	this->getButton(this->_selectedId)->setSelect(true);
}

void RustyWindow::setCursor(unsigned int id)
{
	for (auto button : this->_buttons) {
		button->setSelect(false);
	}

	if (id > 0 && id < this->_buttonIdCounter) {
		this->_selectedId = id;
	}

	this->getButton(this->_selectedId)->setSelect(true);
}

void RustyWindow::move(int vecx, int vecy)
{
	if (this->_position->x + this->_position->w + vecx < this->_screenSize->Width && this->_position->x > 0) {
		this->_position->x += vecx;

		if (this->_position->x < 1) {
			this->_position->x = 1;
		}
		if (this->_position->x + this->_position->w > this->_screenSize->Width) {
			this->_position->x = this->_screenSize->Width - this->_position->w - 1;
		}
	}

	if (this->_position->y + this->_position->h + vecy < this->_screenSize->Height && this->_position->y > 0) {
		this->_position->y += vecy;

		if (this->_position->y < 1) {
			this->_position->y = 1;
		}
		if (this->_position->y + this->_position->h > this->_screenSize->Height) {
			this->_position->y = this->_screenSize->Height - this->_position->h - 1;
		}
	}
}

void RustyWindow::draw()
{
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);
	SDL_Color oldColor; SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

	SDL_SetRenderTarget(this->_renderer, NULL);
	
	if (this->_customTexture) {
		SDL_RenderCopy(this->_renderer, this->_customTexture, NULL, this->_position);
	}
	else {
		SDL_SetRenderDrawColor(this->_renderer, this->_backgroundColor.r, this->_backgroundColor.g, this->_backgroundColor.b, this->_backgroundColor.a);
		SDL_RenderFillRect(this->_renderer, this->_position);
		SDL_SetRenderDrawColor(this->_renderer, this->_borderColor.r, this->_borderColor.g, this->_borderColor.b, this->_borderColor.a);
		SDL_RenderDrawRect(this->_renderer, this->_position);
	}

	SDL_Rect tempRect;
	for (auto text : this->_texts) {
		tempRect.x = text->rect->x + this->_position->x;
		tempRect.y = text->rect->y + this->_position->y;
		tempRect.w = text->rect->w;
		tempRect.h = text->rect->h;
		SDL_RenderCopy(this->_renderer, text->texture, NULL, &tempRect);
	}

	for (auto button : this->_buttons) {
		button->draw(this->_position);
	}

	SDL_SetRenderTarget(this->_renderer, oldTarget);
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.b, oldColor.b, oldColor.a);
}

int RustyWindow::enter()
{
	auto button = this->getButton(this->getSelectedId());
	if (button) {
		return button->makeFunction();
	}
	return -1;
}

RustyWindow::~RustyWindow()
{
	delete this->_position;
	SDL_DestroyTexture(this->_customTexture);
	
	for (auto text : this->_texts) {
		delete text;
	}

	for (auto button : this->_buttons) {
		delete button;
	}
}