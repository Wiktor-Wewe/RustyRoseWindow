#pragma once
#include "SharedResourceUtil.h"
#include "RustyButton.h"

class RustyWindow
{
public:
	explicit RustyWindow(SDL_Renderer* renderer, ScreenSize* screenSize, TTF_Font* font, int width, int height);
	void setFont(TTF_Font* font);
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setBackgroundColor(SDL_Color color);
	void setBorderColor(SDL_Color color);
	void setFontColor(SDL_Color color);
	void setFontOutlineColor(SDL_Color color);
	void setBarColor(SDL_Color color);
	void setSelectHoverColor(SDL_Color color);
	void setCustomTexture(SDL_Texture* texture);
	void setBarSize(int height);
	
	void addText(std::string text, int x, int y, TTF_Font* font = NULL);
	void removeText(std::string text);
	void centerTexts();

	unsigned int addButton(std::string text, int x, int y, int width, int height, TTF_Font* font = NULL); // windowMenager sould manage id
	void removeButton(int id);
	void centerButtons();
	RustyButton* getButton(int id);

	void updateSelectedId(int mouseX, int mouseY);
	int click();
	int getSelectedId();

	SDL_Rect* getWindowPosition();
	SDL_Rect* getBarPosition();
	SDL_Rect* getBarAndWindowPosition();

	void moveCursor(int direction);
	void setCursor(unsigned int id);

	void move(int vecx, int vecy);
	virtual void draw();
	int enter();

	~RustyWindow();

protected:
	unsigned int _buttonIdCounter;
	SDL_Renderer* _renderer;
	ScreenSize* _screenSize;
	TTF_Font* _font;
	SDL_Rect* _position;
	SDL_Rect* _barPosition;
	SDL_Rect* _barAndWindowPosition; // only updated when getBarAndWindowPosition()
	int _selectedId;

	SDL_Color _backgroundColor;
	SDL_Color _borderColor;
	SDL_Color _fontColor;
	SDL_Color _fontOutlineColor;
	SDL_Color _barColor;
	SDL_Color _selectHoverColor;

	SDL_Texture* _customTexture;
	std::vector<ImageText*> _texts;
	std::vector<RustyButton*> _buttons;
};

