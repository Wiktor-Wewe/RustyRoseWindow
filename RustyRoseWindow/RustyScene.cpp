#include "RustyScene.h"

RustyScene::RustyScene(SDL_Renderer* renderer, TTF_Font* font, ScreenSize* screenSize)
{
	this->_renderer = renderer;
	this->_font = font;
	this->_screenSize = screenSize;

	this->_dialogColor = { 0xff, 0xff, 0xff, 0xff };
	this->_dialogHoverColor = { 0x00, 0x00, 0x00, 0xff };
	this->_maxLineSize = this->_screenSize->Width * 0.9; // check if works fine
}

void RustyScene::setFont(TTF_Font* font)
{
	this->_font = font;
}

void RustyScene::setDialogColor(SDL_Color color)
{
	this->_dialogColor = color;
}

void RustyScene::setDialogHoverColor(SDL_Color color)
{
	this->_dialogHoverColor = color;
}

void RustyScene::setMaxLineSize(int max)
{
	this->_maxLineSize = max;
}


void RustyScene::addDialog(std::string dialog)
{
	ImageText* imageText = new ImageText;
	imageText->text = dialog;
	imageText->texture = makeTextureFromText(dialog, imageText->rect, this->_font, this->_dialogColor, this->_dialogHoverColor, this->_renderer, this->_maxLineSize);
	imageText->rect->x = (this->_screenSize->Width / 2) - (imageText->rect->w / 2);

	imageText->rect->y = this->_screenSize->Height - imageText->rect->h - 10;
	if (this->_dialogs.empty() == false) {
		if (this->_dialogs[0]->rect->y + this->_dialogs[0]->rect->h + 10 > imageText->rect->y) {
			for (auto dialog : this->_dialogs) {
				imageText->rect->y -= dialog->rect->h + 10;
			}
		}
	}

	this->_dialogs.push_back(imageText);
}

void RustyScene::removeDialog(std::string dialog)
{
	for (int i = 0; i < this->_dialogs.size(); i++) {
		if (this->_dialogs[i]->text == dialog) {
			delete this->_dialogs[i];
			this->_dialogs.erase(this->_dialogs.begin() + i);
			return;
		}
	}
}

void RustyScene::addText(std::string text, int x, int y, SDL_Color color, SDL_Color hoverColor, TTF_Font* font, int maxLineSize)
{
	ImageText* imageText = new ImageText;
	imageText->text = text;
	imageText->rect->x = x;
	imageText->rect->y = y;

	TTF_Font* selectedFont = font != nullptr ? font : this->_font;
	int selectedMaxLineSize = maxLineSize != 0 ? maxLineSize : this->_maxLineSize;

	imageText->texture = makeTextureFromText(text, imageText->rect, font, color, hoverColor, this->_renderer, selectedMaxLineSize);

	this->_floatingText.push_back(imageText);
}

void RustyScene::removeText(std::string text)
{
	for (int i = 0; i < this->_floatingText.size(); i++) {
		if (this->_floatingText[i]->text == text) {
			delete this->_floatingText[i];
			this->_floatingText.erase(this->_floatingText.begin() + i);
			return;
		}
	}
}

void RustyScene::addImage(std::string path, SDL_Texture* texture, SDL_Rect* rect, int layer)
{
	if (layer < 0) layer = 0;
	if (layer > 2) layer = 2;

	Image* image = new Image;
	image->path = path;
	image->texture = texture;
	image->rect = rect;

		 if (layer == 0) this->_layer0.push_back(image);
	else if (layer == 1) this->_layer1.push_back(image);
	else if (layer == 2) this->_layer2.push_back(image);
}

void RustyScene::removeImage(std::string path)
{
	// layer 0
	for (int i = 0; i < this->_layer0.size(); i++) {
		if (this->_layer0[i]->path == path) {
			this->_layer0.erase(this->_layer0.begin() + i);
			return;
		}
	}

	// layer 1
	for (int i = 0; i < this->_layer1.size(); i++) {
		if (this->_layer1[i]->path == path) {
			this->_layer1.erase(this->_layer1.begin() + i);
			return;
		}
	}

	// layer 2
	for (int i = 0; i < this->_layer2.size(); i++) {
		if (this->_layer2[i]->path == path) {
			this->_layer2.erase(this->_layer2.begin() + i);
			return;
		}
	}
}

void RustyScene::draw()
{
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	for (auto image : this->_layer0) {
		SDL_RenderCopy(this->_renderer, image->texture, NULL, image->rect);
	}

	for (auto image : this->_layer1) {
		SDL_RenderCopy(this->_renderer, image->texture, NULL, image->rect);
	}

	for (auto image : this->_layer2) {
		SDL_RenderCopy(this->_renderer, image->texture, NULL, image->rect);
	}

	for (auto dialog : this->_dialogs) {
		SDL_RenderCopy(this->_renderer, dialog->texture, NULL, dialog->rect);
	}

	for (auto text : this->_floatingText) {
		SDL_RenderCopy(this->_renderer, text->texture, NULL, text->rect);
	}

	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

void RustyScene::clear(Clear option)
{
	if (option == Clear::Layer0) {
		this->_layer0.clear();
	}
	else if (option == Clear::Layer1) {
		this->_layer1.clear();
	}
	else if (option == Clear::Layer2) {
		this->_layer2.clear();
	}
	else if (option == Clear::AllLayers) {
		this->_layer0.clear();
		this->_layer1.clear();
		this->_layer2.clear();
	}
	else if (option == Clear::FloatingText) {
		for (auto text : this->_floatingText) {
			delete text;
		}
		this->_floatingText.clear();
	}
	else if (option == Clear::Dialogs) {
		for (auto dialog : this->_dialogs) {
			delete dialog;
		}
		this->_dialogs.clear();
	}
	else if (option == Clear::All) {
		this->_layer0.clear();
		this->_layer1.clear();
		this->_layer2.clear();

		for (auto text : this->_floatingText) {
			delete text;
		}
		this->_floatingText.clear();

		for (auto dialog : this->_dialogs) {
			delete dialog;
		}
		this->_dialogs.clear();
	}
}

RustyScene::~RustyScene()
{
	this->clear(Clear::All);
}
