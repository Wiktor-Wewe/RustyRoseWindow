#include "RustyWindow.h"

RustyWindow::RustyWindow(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font, int width, int height)
{
	this->_show = true;
	this->_showBar = true;
	this->_lockPosition = false;
	this->_lockButtons = false;

	this->_closeWindowButtonId = 0;
	this->_buttonIdCounter = 1;
	this->_renderer = renderer;
	this->_screenSize = screenSize;
	this->_font = font;

	this->_position = new SDL_Rect;
	this->_position->w = width;
	this->_position->h = height;

	this->_customTexture = NULL;
	
	this->_position->x = (this->_screenSize->Width / 2) - (this->_position->w / 2);
	this->_position->y = (this->_screenSize->Height / 2) - (this->_position->h / 2);

	this->_barPosition = new SDL_Rect;
	this->_barPosition->w = this->_position->w;
	this->_barPosition->h = 30;
	this->_barPosition->x = this->_position->x;
	this->_barPosition->y = this->_position->y - this->_barPosition->h + 1;

	this->_barAndWindowPosition = new SDL_Rect;

	this->_selectedId = 0;
	this->_backgroundColor = { 0x00, 0x00, 0x00, 0xff };
	this->_borderColor = { 0xff, 0x99, 0x00, 0xff };
	this->_fontColor = { 0xff, 0xff, 0xff,0xff };
	this->_fontOutlineColor = { 0xff, 0x99, 0x00, 0xff };
	this->_barColor = { 0x00, 0x00, 0x00, 0xff };
	this->_selectHoverColor = { 0xff, 0x00, 0x00, 0xff };
}

void RustyWindow::setFont(RRW_Font* font)
{
	this->_font = font;
}

void RustyWindow::setPosition(int x, int y)
{
	this->_position->x = x;
	this->_position->y = y;

	this->_barPosition->x = this->_position->x;
	this->_barPosition->y = this->_position->y - this->_barPosition->h + 1;
}

void RustyWindow::setSize(int width, int height)
{
	if (width > 0) {
		this->_position->w = width;
		this->_barPosition->w = this->_position->w;
	}

	if (height > 0) {
		this->_position->h = height;
	}
}

void RustyWindow::setBackgroundColor(SDL_Color color)
{
	this->_backgroundColor = color;
}

void RustyWindow::setBorderColor(SDL_Color color)
{
	this->_borderColor = color;
}

void RustyWindow::setFontColor(SDL_Color color)
{
	this->_fontColor = color;
}

void RustyWindow::setFontOutlineColor(SDL_Color color)
{
	this->_fontOutlineColor = color;
}

void RustyWindow::setBarColor(SDL_Color color)
{
	this->_barColor = color;
}

void RustyWindow::setSelectHoverColor(SDL_Color color)
{
	this->_selectHoverColor = color;
}

void RustyWindow::setCustomTexture(SDL_Texture* texture)
{
	this->_customTexture = texture;
}

void RustyWindow::setBarSize(int height)
{
	this->_barPosition->h = height;
	this->_barPosition->y = this->_position->y - this->_barPosition->h + 1;
}

bool RustyWindow::isFontSet()
{
	if (this->_font) {
		return true;
	}
	return false;
}

void RustyWindow::addText(std::string text, int x, int y, RRW_Font* font, int maxWidth)
{
	RRW_ImageText* imageText = new RRW_ImageText;
	imageText->text = text;
	imageText->rect->x = x;
	imageText->rect->y = y;

	auto windowFont = font == NULL ? this->_font : font;
	if (maxWidth == 0) maxWidth = this->_position->w - 10;

	imageText->texture = RRW_MakeTextureFromText(text, imageText->rect, windowFont, this->_fontColor, this->_fontOutlineColor, this->_renderer, maxWidth);

	this->_texts.push_back(imageText);
}

void RustyWindow::removeText(std::string text)
{
	for (int i = 0; i < this->_texts.size(); i++) {
		if (this->_texts[i]->text == text) {
			delete this->_texts[i];
			this->_texts.erase(this->_texts.begin() + i);
			return;
		}
	}
}

void RustyWindow::centerTexts()
{
	int partOfScreenY = (this->_position->h * 0.83) / (this->_texts.size() + 1);

	for (int i = 0; i < this->_texts.size(); i++) {
		this->_texts[i]->rect->x = (this->_position->w / 2) - (this->_texts[i]->rect->w / 2);
		this->_texts[i]->rect->y = (partOfScreenY + (i * partOfScreenY)) - (this->_texts[i]->rect->h / 2);
	}
}

unsigned int RustyWindow::addButton(std::string text, int x, int y, int width, int height, RRW_Font* font)
{
	// set font and id
	RRW_Font* buttonFont = font == NULL ? this->_font : font;
	unsigned int id = this->_buttonIdCounter;

	// add button and text on it
	RustyButton* button = new RustyButton(id, this->_renderer, this->_screenSize, buttonFont, x, y, width, height);
	button->setText(text);
	this->_buttons.push_back(button);

	// move counter and return id of new button
	this->_buttonIdCounter++;
	return id;
}

unsigned int RustyWindow::addCloseButton()
{
	unsigned int id = this->_buttonIdCounter;

	SDL_Rect buttonPosition;
	int buttonSize = this->_barPosition->h * 0.8;
	buttonPosition.x = this->_barPosition->w - buttonSize - (buttonSize * 0.2);
	buttonPosition.y = -this->_barPosition->h + buttonSize * 0.15;
	buttonPosition.w = buttonSize;
	buttonPosition.h = buttonSize;

	auto button = new RustyButton(id, this->_renderer, this->_screenSize, this->_font, buttonPosition.x, buttonPosition.y, buttonPosition.w, buttonPosition.h);
	button->setText("X");
	this->_buttons.push_back(button);
	button->setFunction([this]()-> int { return this->_closeWindow(); });
	this->_closeWindowButtonId = id;

	this->_buttonIdCounter++;
	return id;
}

void RustyWindow::lockButtons()
{
	this->_lockButtons = true;
}

void RustyWindow::unlockButtons()
{
	this->_lockButtons = false;
}

bool RustyWindow::isButtonsLocked()
{
	return this->_lockButtons;
}

void RustyWindow::removeButton(int id)
{
	for (int i = 0; i < this->_buttons.size(); i++) {
		if (this->_buttons[i]->getId() == id) {
			delete this->_buttons[i];
			this->_buttons.erase(this->_buttons.begin() + i);
			return;
		}
	}
}

void RustyWindow::centerButtons()
{
	std::vector<RustyButton*> buttons;
	for (auto button : this->_buttons) {
		if (button->getId() != this->_closeWindowButtonId) {
			buttons.push_back(button);
		}
	}

	int partOfScreenX = this->_position->w / (buttons.size() + 1);
	int partOfScreenY = this->_position->h / 6;

	for (int i = 0; i < buttons.size(); i++) {
		int tempX = (partOfScreenX + (partOfScreenX * i)) - ((buttons[i]->getPosition()->w) / 2);
		buttons[i]->setPosition(tempX, (partOfScreenY * 5) - (buttons[i]->getPosition()->h / 2));
	}
}

void RustyWindow::formatButtons()
{
	int maxWidth = 0;
	int maxHeight = 0;

	for (auto button : this->_buttons) {
		if (maxWidth < button->getPosition()->w) {
			maxWidth = button->getPosition()->w;
		}
		if (maxHeight < button->getPosition()->h) {
			maxHeight = button->getPosition()->h;
		}
	}

	for (auto button : this->_buttons) {
		button->setSize(maxWidth, maxHeight);
		button->centerText();
	}
}

RustyButton* RustyWindow::getButton(int id)
{
	for (auto button : this->_buttons) {
		if (button->getId() == id) {
			return button;
		}
	}

	return nullptr;
}

void RustyWindow::updateSelectedId(int mouseX, int mouseY)
{
	SDL_Rect temp;
	bool set = false;

	for (auto button : this->_buttons) {
		button->setSelect(false);
		temp.x = button->getPosition()->x + this->_position->x;
		temp.y = button->getPosition()->y + this->_position->y;
		temp.w = button->getPosition()->w;
		temp.h = button->getPosition()->h;
		if (RRW_CheckMousePositionOnObject(mouseX, mouseY, &temp)) {
			this->_selectedId = button->getId();
			button->setSelect(true);
			set = true;
		}
	}

	if (!set) {
		this->_selectedId = 0;
	}
}

int RustyWindow::click()
{
	if (this->_lockButtons) {
		return -2;
	}

	if (this->getSelectedId() == this->_closeWindowButtonId && this->_showBar == false) {
		return -2;
	}

	auto button = this->getButton(this->getSelectedId());
	if (button) {
		return button->makeFunction();
	}

	return -2;
}

int RustyWindow::getSelectedId()
{
	return this->_selectedId;
}

SDL_Rect* RustyWindow::getWindowPosition()
{
	return this->_position;
}

SDL_Rect* RustyWindow::getBarPosition()
{
	return this->_barPosition;
}

SDL_Rect* RustyWindow::getBarAndWindowPosition()
{
	this->_barAndWindowPosition->x = this->_barPosition->x;
	this->_barAndWindowPosition->y = this->_barPosition->y;
	this->_barAndWindowPosition->w = this->_barPosition->w;
	this->_barAndWindowPosition->h = this->_barPosition->h + this->_position->h;
	return this->_barAndWindowPosition;
}

void RustyWindow::formatWindow()
{
	if (this->_buttons.empty()) {
		return;
	}

	int newWidth = (this->_buttons.size() * this->_buttons[0]->getPosition()->w) + (this->_buttons[0]->getPosition()->w * 1.5);
	this->setSize(newWidth, 0);
	this->centerButtons();
	this->centerTexts();
}

void RustyWindow::centerWindow()
{
	this->_position->x = (this->_screenSize->Width / 2) - (this->_position->w / 2);
	this->_barPosition->x = this->_position->x;
	this->_position->y = (this->_screenSize->Height / 2) - (this->_position->h / 2);
	this->_barPosition->y = this->_position->y - this->_barPosition->h + 1;
}

void RustyWindow::moveCursor(int direction)
{
	for (auto button : this->_buttons) {
		button->setSelect(false);
	}

	if (this->_selectedId + direction > 0 && this->_selectedId + direction < this->_buttonIdCounter) {
		this->_selectedId += direction;
	}

	this->getButton(this->_selectedId)->setSelect(true);
}

void RustyWindow::setCursor(unsigned int id)
{
	for (auto button : this->_buttons) {
		button->setSelect(false);
	}

	if (id > 0 && id < this->_buttonIdCounter) {
		this->_selectedId = id;
	}

	this->getButton(this->_selectedId)->setSelect(true);
}

void RustyWindow::hide()
{
	this->_show = false;
}

void RustyWindow::show()
{
	this->_show = true;
}

void RustyWindow::hideBar()
{
	this->_showBar = false;
}

void RustyWindow::showBar()
{
	this->_showBar = true;
}

void RustyWindow::lockPosition()
{
	this->_lockPosition = true;
}

void RustyWindow::unlockPosition()
{
	this->_lockPosition = false;
}

void RustyWindow::move(int vecx, int vecy)
{
	if (this->_lockPosition) {
		return;
	}

	if (this->_position->x + this->_position->w + vecx < this->_screenSize->Width && this->_position->x > 0) {
		this->_position->x += vecx;

		if (this->_position->x < 1) {
			this->_position->x = 1;
		}
		if (this->_position->x + this->_position->w > this->_screenSize->Width) {
			this->_position->x = this->_screenSize->Width - this->_position->w - 1;
		}
	}

	if (this->_position->y + this->_position->h + vecy < this->_screenSize->Height && this->_position->y > 0) {
		this->_position->y += vecy;

		if (this->_position->y < 1) {
			this->_position->y = 1;
		}
		if (this->_position->y + this->_position->h > this->_screenSize->Height) {
			this->_position->y = this->_screenSize->Height - this->_position->h - 1;
		}
	}

	this->_barPosition->x = this->_position->x;
	this->_barPosition->y = this->_position->y - this->_barPosition->h + 1;
}

void RustyWindow::draw()
{
	if (this->_show == false) {
		return;
	}

	auto oldTarget = SDL_GetRenderTarget(this->_renderer);
	SDL_Color oldColor; SDL_GetRenderDrawColor(this->_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

	SDL_SetRenderTarget(this->_renderer, NULL);
	
	if (this->_customTexture) {
		SDL_RenderCopy(this->_renderer, this->_customTexture, NULL, this->_position);
	}
	else {
		SDL_SetRenderDrawColor(this->_renderer, this->_backgroundColor.r, this->_backgroundColor.g, this->_backgroundColor.b, this->_backgroundColor.a);
		SDL_RenderFillRect(this->_renderer, this->_position);
		SDL_SetRenderDrawColor(this->_renderer, this->_borderColor.r, this->_borderColor.g, this->_borderColor.b, this->_borderColor.a);
		SDL_RenderDrawRect(this->_renderer, this->_position);
		if (this->_showBar) {
			SDL_SetRenderDrawColor(this->_renderer, this->_barColor.r, this->_barColor.g, this->_barColor.b, this->_barColor.a);
			SDL_RenderFillRect(this->_renderer, this->_barPosition);
			SDL_SetRenderDrawColor(this->_renderer, this->_borderColor.r, this->_borderColor.g, this->_borderColor.b, this->_borderColor.a);
			SDL_RenderDrawRect(this->_renderer, this->_barPosition);
		}
	}

	SDL_Rect tempRect;
	for (auto text : this->_texts) {
		tempRect.x = text->rect->x + this->_position->x;
		tempRect.y = text->rect->y + this->_position->y;
		tempRect.w = text->rect->w;
		tempRect.h = text->rect->h;
		SDL_RenderCopy(this->_renderer, text->texture, NULL, &tempRect);
	}

	for (auto button : this->_buttons) {
		if (this->_showBar == false && button->getId() == this->_closeWindowButtonId) {
			continue;
		}
		button->draw(this->_position);
	}

	SDL_SetRenderTarget(this->_renderer, oldTarget);
	SDL_SetRenderDrawColor(this->_renderer, oldColor.r, oldColor.b, oldColor.b, oldColor.a);
}

RustyWindow::~RustyWindow()
{
	delete this->_position;
	delete this->_barPosition;
	delete this->_barAndWindowPosition;
	SDL_DestroyTexture(this->_customTexture);
	
	for (auto text : this->_texts) {
		delete text;
	}

	for (auto button : this->_buttons) {
		delete button;
	}
}

int RustyWindow::_closeWindow()
{
	return -1;
}
