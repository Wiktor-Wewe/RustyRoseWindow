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

	RustyScene(SDL_Renderer* renderer, RRW_Font* font, RRW_ScreenSize* screenSize);
	void setFont(RRW_Font* font);
	void setDialogColor(SDL_Color color);
	void setDialogHoverColor(SDL_Color color);
	void setMaxLineSize(int max);

	void addDialog(std::string dialog);
	void removeDialog(std::string dialog);

	void addText(std::string text, int x, int y, SDL_Color color, SDL_Color hoverColor, RRW_Font* font = NULL, int maxLineSize = 0);
	void removeText(std::string text);

	void addImage(std::string path, SDL_Texture* texture, SDL_Rect* rect, int layer = 0);
	void removeImage(std::string path);

	void draw();
	void clear(Clear option);

	~RustyScene();

private:
	SDL_Renderer* _renderer;
	RRW_Font* _font;
	RRW_ScreenSize* _screenSize;
	SDL_Color _dialogColor;
	SDL_Color _dialogHoverColor;
	int _maxLineSize;

	std::vector<RRW_Image*> _layer0;
	std::vector<RRW_Image*> _layer1;
	std::vector<RRW_Image*> _layer2;

	std::vector<RRW_ImageText*> _floatingText;
	std::vector<RRW_ImageText*> _dialogs;
};

