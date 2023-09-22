#pragma once
#include "SharedResourceUtil.h"
#include "RustyButton.h"

class RustyMiniWindow
{
public :
	RustyMiniWindow(int width, int height, TTF_Font* defaultFont, SDL_Renderer* renderer, ScreenSize* screenSize);
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setDefaultFont(TTF_Font* font);
	void setBackGroundColor(SDL_Color color);
	void setWindowHoverColor(SDL_Color color);
	void setFontColor(SDL_Color color);
	void setFontHoverColor(SDL_Color color);
	void setCustomTexture(SDL_Texture* texture);

	void addText(std::string text, int x, int y, TTF_Font* font = NULL);
	void removeText(std::string text);

	void addButton(std::string text, int id, int width, int height, TTF_Font* font = NULL);
	void removeButton(int id);
	RustyButton* getButton(int id);

	void selectId(int id);
	void move(int vecx, int vecy);
	void make(); // edit make -> make only text and bg and after this draw buttons
	void draw();

	~RustyMiniWindow();

private:
	int _selectedId;
	SDL_Rect* _position;
	SDL_Renderer* _renderer;
	TTF_Font* _defaultFont;
	ScreenSize* _screenSize;

	SDL_Color _backGroundColor;
	SDL_Color _windowHoverColor;
	SDL_Color _fontColor;
	SDL_Color _fontHoverColor;

	std::vector<ImageText*> _text;
	std::vector<RustyButton*> _buttons;

	SDL_Texture* _texture;
	SDL_Texture* _customTexture;
};

