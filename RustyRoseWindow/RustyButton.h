#pragma once
#include "SharedResourceUtil.h"
#include <functional>

class RustyButton
{
public:
	RustyButton(int id, SDL_Renderer* renderer, ScreenSize* screenSize, TTF_Font* font, int x, int y, int width, int height);
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setText(std::string text);
	void setFont(TTF_Font* font);
	void setFontColor(SDL_Color color);
	void setFontOutlineColor(SDL_Color color);
	void setTextPosition(int x, int y);
	void setBackgroundColor(SDL_Color color);
	void setBorderColor(SDL_Color color);
	void setSelectHoverColor(SDL_Color color);
	void setSelect(bool isSelected);

	void setFunction(std::function<int()> function);
	int makeFunction();

	void setCustomTexture(SDL_Texture* texture);
	void setCustomHoverTexture(SDL_Texture* texture);
	
	int getId();
	void move(int vecx, int vecy);
	void draw(SDL_Rect* windowPosition);

	~RustyButton();

private:
	int _id;

	SDL_Renderer* _renderer;
	ScreenSize* _screenSize;
	TTF_Font* _font;
	SDL_Rect* _position;
	ImageText* _text;
	bool _selected;

	std::function<int()> _function;

	SDL_Color _backgroundColor;
	SDL_Color _borderColor;
	SDL_Color _fontColor;
	SDL_Color _fontOutlineColor;
	SDL_Color _selectHoverColor;

	SDL_Texture* _customTexture;
	SDL_Texture* _customHoverTexture;
};

