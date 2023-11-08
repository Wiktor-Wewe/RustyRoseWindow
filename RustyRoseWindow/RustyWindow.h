#pragma once
#include "SharedResourceUtil.h"
#include "RustyButton.h"

class RustyWindow
{
public:
	explicit RustyWindow(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font, int width, int height);
	void setFont(RRW_Font* font);
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
	
	bool isFontSet();
	void addText(std::string text, int x, int y, RRW_Font* font = NULL, int maxWidth = 0);
	void removeText(std::string text);
	void centerTexts();

	unsigned int addButton(std::string text, int x, int y, int width, int height, RRW_Font* font = NULL); // windowManager should manage id
	unsigned int addCloseButton();
	void lockButtons();
	void unlockButtons();
	bool isButtonsLocked();
	void removeButton(int id);
	void centerButtons();
	void formatButtons();
	RustyButton* getButton(int id);

	void updateSelectedId(int mouseX, int mouseY);
	int click();
	int getSelectedId();

	SDL_Rect* getWindowPosition();
	SDL_Rect* getBarPosition();
	SDL_Rect* getBarAndWindowPosition();
	void formatWindow();
	void centerWindow();

	void moveCursor(int direction);
	void setCursor(unsigned int id);

	void hide();
	void show();

	void hideBar();
	void showBar();

	void lockPosition();
	void unlockPosition();

	void move(int vecx, int vecy);
	virtual void draw();

	~RustyWindow();

protected:
	bool _show;
	bool _showBar;
	bool _lockPosition;
	bool _lockButtons;

	unsigned int _closeWindowButtonId;
	unsigned int _buttonIdCounter;
	SDL_Renderer* _renderer;
	RRW_ScreenSize* _screenSize;
	RRW_Font* _font;
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
	std::vector<RRW_ImageText*> _texts;
	std::vector<RustyButton*> _buttons;

	int _closeWindow();
};

