#pragma once
#include "SharedResourceUtil.h"

class RustyDialogWindow
{
public:
	RustyDialogWindow(std::string text, int windowWidth, int windowHeight, TTF_Font* textFont, TTF_Font* buttonFont, SDL_Renderer* renderer, ScreenSize screenSize);

	void setBackgroundColor(SDL_Color color);
	void setHoverColor(SDL_Color color);
	void setFontColor(SDL_Color color);
	void setSelectColor(SDL_Color color);
	void setTextFont(TTF_Font* font);
	void setButtonFont(TTF_Font* font);

	void setCustomTexture(SDL_Texture* texture);
	void setCustomHoverTexture(SDL_Texture* texture);
	void setCustomPositionOfButtons(SDL_Rect button1, SDL_Rect button2);

	void move(int vecx, int vecy);
	void changeSelect(int vecx);
	int enter();

	void make();
	void draw();

	~RustyDialogWindow();

private:
	SDL_Renderer* _renderer;
	int _selectIndex;
	bool _end;

	SDL_Texture* _mainTexture; // main texture without hover
	SDL_Texture* _mainTextureHover; // only hover to main texture

	std::string _text;
	SDL_Rect _position;

	SDL_Color _backGroundColor;
	SDL_Color _hoverColor;
	SDL_Color _fontColor;
	SDL_Color _selectColor;

	TTF_Font* textFont;
	TTF_Font* buttonFont;

	SDL_Rect _positionButton1;
	SDL_Rect _positionButton2;

	SDL_Texture* _texture;
	SDL_Texture* _textureHover;

	void _updateHoverTexture();
};

