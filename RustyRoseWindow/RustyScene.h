#pragma once
#include "SharedResourceUtil.h"

class RustyScene
{
public:
	enum Clear {
		Layer0,
		Layer1,
		Layer2,
		AllLayers,
		FloatingText,
		Dialogs,
		All
	};

	RustyScene(SDL_Renderer* renderer, TTF_Font* font, ScreenSize* screenSize);
	void setFont(TTF_Font* font);
	void setDialogColor(SDL_Color color);
	void setDialogHoverColor(SDL_Color color);
	void setMaxLineSize(int max);

	void addDialog(std::string dialog);
	void removeDialog(std::string dialog);

	void addText(std::string text, int x, int y, SDL_Color color, SDL_Color hoverColor, TTF_Font* font = nullptr, int maxLineSize = 0);
	void removeText(std::string text);

	void addImage(std::string path, SDL_Texture* texture, SDL_Rect* rect, int layer = 0);
	void removeImage(std::string path);

	void draw();
	void clear(Clear option);

	~RustyScene();

private:
	SDL_Renderer* _renderer;
	TTF_Font* _font;
	ScreenSize* _screenSize;
	SDL_Color _dialogColor;
	SDL_Color _dialogHoverColor;
	int _maxLineSize;

	std::vector<Image*> _layer0;
	std::vector<Image*> _layer1;
	std::vector<Image*> _layer2;

	std::vector<ImageText*> _floatingText;
	std::vector<ImageText*> _dialogs;
};

