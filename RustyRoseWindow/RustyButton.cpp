#include "RustyButton.h"

RustyButton::RustyButton(SDL_Renderer* renderer, TTF_Font* font, int id, std::string text, int width, int height, ScreenSize* screenSize)
{
	this->_renderer = renderer;
	this->_id = id;
	this->_isSelected = false;
	this->_text = text;
	this->_position = new SDL_Rect;
	this->_position->w = width;
	this->_position->h = height;
	this->_screenSize = screenSize;

	this->_position->x = (this->_screenSize->Width / 2) - (this->_position->w / 2);
	this->_position->y = (this->_screenSize->Height / 2) - (this->_position->h / 2);

	this->_font = font;
	this->_textColor = { 0xff, 0xff, 0xff, 0xff };
	this->_textHoverColor = { 0xff, 0x99, 0x00, 0xff };
	this->_textPosition = new SDL_Rect;
	this->_textPosition->x = 0;
	this->_textPosition->y = 0;

	this->_backGroundColor = { 0x00, 0x00, 0x00, 0xff };
	this->_hoverColor = { 0xff, 0x99, 0x00, 0xff };
	this->_selectHoverColor = { 0xff, 0x00, 0x00, 0xff };

	this->_mainTexture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->_position->w, this->_position->h);
	this->_hoverTexture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->_position->w, this->_position->h);
	SDL_SetTextureBlendMode(this->_hoverTexture, SDL_BLENDMODE_BLEND);
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
	this->_text = text;
}

void RustyButton::setFont(TTF_Font* font)
{
	this->_font = font;
}

void RustyButton::setTextColor(SDL_Color color)
{
	this->_textColor = color;
}

void RustyButton::setTextHoverColor(SDL_Color color)
{
	this->_textHoverColor = color;
}

void RustyButton::setTextPosition(int x, int y)
{
	this->_textPosition->x = x;
	this->_textPosition->y = y;
}

void RustyButton::setBackGroundColor(SDL_Color color)
{
	this->_backGroundColor = color;
}

void RustyButton::setHoverColor(SDL_Color color)
{
	this->_hoverColor = color;
}

void RustyButton::setSelectHoverColor(SDL_Color color)
{
	this->_selectHoverColor = color;
}

void RustyButton::setHover(bool isSelected)
{
	this->_isSelected = isSelected;
}

void RustyButton::make()
{
	// get old parameters
	SDL_Color oldColor;
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);
	SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

	// draw backgorund
	SDL_SetRenderTarget(this->_renderer, this->_mainTexture);
	SDL_SetRenderDrawColor(this->_renderer, this->_backGroundColor.r, this->_backGroundColor.g, this->_backGroundColor.b, this->_backGroundColor.a);
	SDL_RenderClear(this->_renderer);

	// draw text and hover on main texture
	SDL_Texture* textTexture = makeTextureFromText(this->_text, this->_textPosition, this->_font, this->_textColor, this->_textHoverColor, this->_renderer, this->_position->w);
	this->_textPosition->x = (this->_position->w / 2) - (this->_textPosition->w / 2);
	this->_textPosition->y = (this->_position->h / 2) - (this->_textPosition->h / 2);
	SDL_RenderCopy(this->_renderer, textTexture, NULL, this->_textPosition);
	// hover
	SDL_SetRenderTarget(this->_renderer, this->_mainTexture);
	SDL_RenderCopy(this->_renderer, this->_mainTexture, NULL, NULL);
	SDL_SetRenderDrawColor(this->_renderer, this->_hoverColor.r, this->_hoverColor.g, this->_hoverColor.b, this->_hoverColor.a);
	SDL_Rect* hoverPosition = new SDL_Rect { 0, 0, this->_position->w, this->_position->h };
	SDL_RenderDrawRect(this->_renderer, hoverPosition);
	hoverPosition->x++; hoverPosition->y++; hoverPosition->w--; hoverPosition->h--;
	SDL_RenderDrawRect(this->_renderer, hoverPosition);

	// draw select hover on texture
	SDL_SetRenderTarget(this->_renderer, this->_hoverTexture);
	SDL_RenderCopy(this->_renderer, this->_mainTexture, NULL, NULL);
	SDL_SetRenderDrawColor(this->_renderer, this->_selectHoverColor.r, this->_selectHoverColor.g, this->_selectHoverColor.b, this->_selectHoverColor.a);
	hoverPosition = new SDL_Rect{ 0, 0, this->_position->w, this->_position->h };
	SDL_RenderDrawRect(this->_renderer, hoverPosition);
	hoverPosition->x++; hoverPosition->y++; hoverPosition->w--; hoverPosition->h--;
	SDL_RenderDrawRect(this->_renderer, hoverPosition);

	// delete leftovers
	SDL_DestroyTexture(textTexture);
	delete hoverPosition;
		
	// set old parameters
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.b);
	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

int RustyButton::getId()
{
	return this->_id;
}

SDL_Rect* RustyButton::getPosition()
{
	return this->_position;
}

SDL_Texture* RustyButton::getTexture()
{
	return this->_isSelected ? this->_hoverTexture : this->_mainTexture;
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

RustyButton::~RustyButton()
{
	delete this->_position;
	delete this->_textPosition;
	SDL_DestroyTexture(this->_mainTexture);
	SDL_DestroyTexture(this->_hoverTexture);
}
