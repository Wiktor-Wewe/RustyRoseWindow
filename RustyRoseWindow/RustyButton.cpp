#include "RustyButton.h"

RustyButton::RustyButton(int id, SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font, int x, int y, int width, int height)
{
	this->_id = id;

	this->_renderer = renderer;
	this->_screenSize = screenSize;
	this->_font = font;
	this->_position = new SDL_Rect;
	this->_position->x = x;
	this->_position->y = y;

	this->_position->w = width;
	this->_position->h = height;
	this->_text = NULL;
	this->_selected = false;

	this->_function = NULL;

	this->_backgroundColor = { 0x00, 0x00, 0x00, 0xff };
	this->_borderColor = { 0xff, 0x99, 0x00, 0xff };
	this->_fontColor = { 0xff, 0xff,0xff, 0xff };
	this->_fontOutlineColor = { 0x00, 0x00, 0x00, 0xff };
	this->_selectHoverColor = { 0xff, 0x00, 0x00, 0xff };

	this->_customTexture = NULL;
	this->_customHoverTexture = NULL;
}

void RustyButton::setPosition(int x, int y)
{
	this->_position->x = x;
	this->_position->y = y;
}

void RustyButton::setSize(int width, int height)
{
	this->_position->w = width;
	this->_position->h = height;
}

void RustyButton::setText(std::string text)
{
	if (this->_text) {
		delete this->_text;
		this->_text = NULL;
	}

	RRW_ImageText* imageText = new RRW_ImageText;
	imageText->text = text;

	if (this->_position->w == 0 || this->_position->h == 0) { // <- && ? ||
		imageText->texture = RRW_MakeTextureFromText(text, imageText->rect, this->_font, this->_fontColor, this->_fontOutlineColor, this->_renderer, 0);
		this->_position->w = imageText->rect->w * 1.4;
		this->_position->h = imageText->rect->h * 1.2;
		imageText->rect->x = (this->_position->w / 2) - (imageText->rect->w / 2);
		imageText->rect->y = (this->_position->h / 2) - (imageText->rect->h / 2);
	}
	else {
		imageText->texture = RRW_MakeTextureFromText(text, imageText->rect, this->_font, this->_fontColor, this->_fontOutlineColor, this->_renderer, this->_position->w - 5);
		imageText->rect->x = (this->_position->w / 2) - (imageText->rect->w / 2);
		imageText->rect->y = (this->_position->h / 2) - (imageText->rect->h / 2);
	}
	
	this->_text = imageText;
}

void RustyButton::setFont(RRW_Font* font)
{
	this->_font = font;
}

void RustyButton::setFontColor(SDL_Color color)
{
	this->_fontColor = color;
}

void RustyButton::setFontOutlineColor(SDL_Color color)
{
	this->_fontOutlineColor = color;
}

void RustyButton::setTextPosition(int x, int y)
{
	this->_text->rect->x = x;
	this->_text->rect->y = y;
}

void RustyButton::setBackgroundColor(SDL_Color color)
{
	this->_backgroundColor = color;
}

void RustyButton::setBorderColor(SDL_Color color)
{
	this->_borderColor = color;
}

void RustyButton::setSelectHoverColor(SDL_Color color)
{
	this->_selectHoverColor = color;
}

void RustyButton::setSelect(bool isSelected)
{
	this->_selected = isSelected;
}

void RustyButton::setFunction(std::function<int()> function)
{
	this->_function = function;
}

int RustyButton::makeFunction()
{
	if (this->_function) {
		return this->_function();
	}
	return -2;
}

void RustyButton::centerText()
{
	this->_text->rect->x = (this->_position->w / 2) - (this->_text->rect->w / 2);
	this->_text->rect->y = (this->_position->h / 2) - (this->_text->rect->h / 2);
}

void RustyButton::setCustomTexture(SDL_Texture* texture)
{
	if (this->_customTexture) {
		SDL_DestroyTexture(this->_customTexture);
	}

	this->_customTexture = texture;
}

void RustyButton::setCustomHoverTexture(SDL_Texture* texture)
{
	if (this->_customHoverTexture) {
		SDL_DestroyTexture(this->_customHoverTexture);
	}

	this->_customHoverTexture = texture;
}

int RustyButton::getId()
{
	return this->_id;
}

SDL_Rect* RustyButton::getPosition()
{
	return this->_position;
}

void RustyButton::move(int vecx, int vecy)
{
	if (this->_position->x + this->_position->w + vecx < this->_screenSize->Width && this->_position->x >= 0) {
		this->_position->x += vecx;
	}

	if (this->_position->y + this->_position->h + vecy < this->_screenSize->Height && this->_position->y >= 0) {
		this->_position->y += vecy;
	}
}

void RustyButton::draw(SDL_Rect* windowPosition)
{
	// get old parameters
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);
	SDL_Color oldColor; SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

	// draw button
	SDL_SetRenderTarget(this->_renderer, NULL);

	SDL_Rect temp;
	temp.x = this->_position->x + windowPosition->x;
	temp.y = this->_position->y + windowPosition->y;
	temp.w = this->_position->w;
	temp.h = this->_position->h;

	// draw background
	if (this->_customTexture) {
		SDL_RenderCopy(this->_renderer, this->_customTexture, NULL, &temp);
	}
	else {
		SDL_SetRenderDrawColor(this->_renderer, this->_backgroundColor.r, this->_backgroundColor.g, this->_backgroundColor.b, this->_backgroundColor.a);
		SDL_RenderFillRect(this->_renderer, &temp);
	}

	// draw border / hover
	if (this->_selected) {
		SDL_SetRenderDrawColor(this->_renderer, this->_selectHoverColor.r, this->_selectHoverColor.g, this->_selectHoverColor.b, this->_selectHoverColor.a);
		SDL_RenderDrawRect(this->_renderer, &temp);
	}
	else {
		SDL_SetRenderDrawColor(this->_renderer, this->_borderColor.r, this->_borderColor.g, this->_borderColor.b, this->_borderColor.a);
		SDL_RenderDrawRect(this->_renderer, &temp);
	}

	// draw text
	if (this->_text) {
		temp.x += this->_text->rect->x;
		temp.y += this->_text->rect->y;
		temp.w  = this->_text->rect->w;
		temp.h  = this->_text->rect->h;
		SDL_RenderCopy(this->_renderer, this->_text->texture, NULL, &temp);
	}
	
	// set old parameters
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.b);
	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

RustyButton::~RustyButton()
{
	delete this->_position;
	
	if (this->_text) {
		delete this->_text;
	}

	if (this->_customTexture) {
		SDL_DestroyTexture(this->_customTexture);
	}

	if (this->_customHoverTexture) {
		SDL_DestroyTexture(this->_customHoverTexture);
	}
}