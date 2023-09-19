#pragma once
#include "SharedResourceUtil.h"

class RustyButton
{
public:
	RustyButton(SDL_Renderer* renderer, TTF_Font* font, int id, std::string text, int width, int height, ScreenSize* screenSize);
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setText(std::string text);
	void setFont(TTF_Font* font);
	void setTextColor(SDL_Color color);
	void setTextHoverColor(SDL_Color color);
	void setTextPosition(int x, int y);
	void setBackGroundColor(SDL_Color color);
	void setHoverColor(SDL_Color color);
	void setSelectHoverColor(SDL_Color color);
	void setHover(bool isSelected);

	void setCustomTexture(SDL_Texture* texture);
	void setCustomHoverTexture(SDL_Texture* texture);

	void make();
	
	int getId();
	SDL_Rect* getPosition();
	SDL_Texture* getTexture();

	void move(int vecx, int vecy);

	~RustyButton();

private:
	SDL_Renderer* _renderer;
	ScreenSize* _screenSize;
	bool _isSelected;
	int _id;

	SDL_Rect* _position;

	TTF_Font* _font;
	std::string _text;
	SDL_Color _textColor;
	SDL_Color _textHoverColor;
	SDL_Rect* _textPosition;

	SDL_Color _backGroundColor;
	SDL_Color _hoverColor;
	SDL_Color _selectHoverColor;

	SDL_Texture* _mainTexture;
	SDL_Texture* _hoverTexture;
};

