#pragma once
#include <string>
#include <vector>
#include <utility>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Windows.h>

#ifndef SHARED_RESOURCE_UTIL_H
#define SHARED_RESOURCE_UTIL_H

static void RRW_OpenConsole()
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
}

static void RRW_CloseConsole()
{
	FreeConsole();
}

static void RRW_LogInfo(std::string text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	printf("RRW_INFO: ");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf(text.c_str());
	printf("\n");
}

static void RRW_LogWarning(std::string text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
	printf("RRW_WARNING: ");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf(text.c_str());
	printf("\n");
}

static void RRW_LogError(std::string text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	printf("RRW_ERROR: ");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf(text.c_str());
	printf("\n");
}

struct RRW_Font {
	RRW_Font(const char* path, int size, int outlineSize) {
		this->font = TTF_OpenFont(path, size);
		this->outline = TTF_OpenFont(path, size);
		this->outlineSize = outlineSize;
		TTF_SetFontOutline(this->outline, outlineSize);
	}

	TTF_Font* font = NULL;
	TTF_Font* outline = NULL;
	int outlineSize = 0;

	bool isGood() {
		if (this->font && this->outline) {
			return true;
		}
		return false;
	}

	~RRW_Font() {
		if (this->font) TTF_CloseFont(this->font);
		if (this->outline) TTF_CloseFont(this->outline);
	}
};

struct RRW_Fonts {
	RRW_Font* smallFont = NULL;
	RRW_Font* mediumFont = NULL;
	RRW_Font* largeFont = NULL;

	~RRW_Fonts() {
		if (this->smallFont) delete this->smallFont;
		if (this->mediumFont) delete this->mediumFont;
		if (this->largeFont) delete this->largeFont;
	}
};

struct RRW_ScreenSize {
    RRW_ScreenSize(int width, int height) {
        this->Width = width;
        this->Height = height;
    }

    int Width;
    int Height;
};

struct RRW_ImageText {
	std::string text;
	SDL_Texture* texture = NULL;
	SDL_Rect* rect = new SDL_Rect;

	~RRW_ImageText() {
		if (this->rect) delete this->rect;
		if (this->texture) SDL_DestroyTexture(this->texture);
	}
};

struct RRW_Image {
	std::string path;
	SDL_Texture* texture = NULL;
	SDL_Rect* rect = NULL;
};

struct RRW_MouseInfo {
	int x;
	int y;
	bool clickL;
	bool clickR;
};

struct RRW_MouseMove {
	int vecx;
	int vecy;
};

static RRW_Fonts* RRW_OpenFonts(const char* path, int smallSize, int mediumSize, int largeSize, int outlineSize) 
{
	RRW_Font* smallFont = new RRW_Font(path, smallSize, outlineSize);
	RRW_Font* mediumFont = new RRW_Font(path, mediumSize, outlineSize);
	RRW_Font* largeFont = new RRW_Font(path, largeSize, outlineSize);

	RRW_Fonts* fonts = new RRW_Fonts;
	fonts->smallFont = smallFont;
	fonts->mediumFont = mediumFont;
	fonts->largeFont = largeFont;

	if (smallFont->isGood() && mediumFont->isGood() && largeFont->isGood()) {
		return fonts;
	}

	delete fonts;
	fonts = nullptr;
	return fonts;
}

static SDL_Texture* RRW_MakeTextureFromText(std::string text, SDL_Rect* size, RRW_Font* font, SDL_Color textColor, SDL_Color outlineColor, SDL_Renderer* renderer, int wrapLength, int align = TTF_WRAPPED_ALIGN_CENTER)
{
	TTF_SetFontWrappedAlign(font->font, align);
	TTF_SetFontWrappedAlign(font->outline, align);
	SDL_Surface* textOutlineSurface = TTF_RenderUTF8_Blended_Wrapped(font->outline, text.c_str(), outlineColor, wrapLength);
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font->font, text.c_str(), textColor, wrapLength);

	SDL_Rect rect = { 0, font->outlineSize, textSurface->w, textSurface->h }; // default for center (TTF_WRAPPED_ALIGN_CENTER)
	
	switch (align) {
	case TTF_WRAPPED_ALIGN_LEFT:
		rect.x = font->outlineSize;
		break;

	case TTF_WRAPPED_ALIGN_RIGHT:
		rect.y = -font->outlineSize; //  to test
		break;
	}
	
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
	SDL_BlitSurface(textSurface, NULL, textOutlineSurface, &rect);
	SDL_FreeSurface(textSurface);

	size->w = textOutlineSurface->w;
	size->h = textOutlineSurface->h;
	
	auto result = SDL_CreateTextureFromSurface(renderer, textOutlineSurface);
	SDL_FreeSurface(textOutlineSurface);
	return result;
}

static SDL_Texture* RRW_MakeTextureFromTextClassic(std::string text, SDL_Rect* size, TTF_Font* font, SDL_Color textColor, SDL_Color outlineColor, SDL_Renderer* renderer, int wrapLength, int align = TTF_WRAPPED_ALIGN_CENTER)
{
	auto oldTarget = SDL_GetRenderTarget(renderer);
	TTF_SetFontWrappedAlign(font, align);

	SDL_Surface* outlineSurface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), outlineColor, wrapLength);
	if (outlineSurface == NULL) {
		RRW_LogError("unable to make outline surface in text: \n" + text);
		return nullptr;
	}

	SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(renderer, outlineSurface);
	if (outlineTexture == NULL) {
		RRW_LogError("unable to make texture from outline surface in text: \n" + text);
		SDL_FreeSurface(outlineSurface);
		return nullptr;
	}

	SDL_Texture* finalTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, outlineSurface->w, outlineSurface->h);
	if (finalTexture == NULL) {
		RRW_LogError("unable to make final texture in text: \n" + text);
		SDL_FreeSurface(outlineSurface);
		return outlineTexture;
	}
	SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

	SDL_Rect rect = { 0, 0, outlineSurface->w, outlineSurface->h };
	SDL_SetRenderTarget(renderer, finalTexture);

	if (size != NULL) {
		size->w = rect.w;
		size->h = rect.h;
	}

	rect.x = -1;
	SDL_RenderCopy(renderer, outlineTexture, NULL, &rect);
	rect.x = 1;
	SDL_RenderCopy(renderer, outlineTexture, NULL, &rect);
	rect.x = 0;
	rect.y = -1;
	SDL_RenderCopy(renderer, outlineTexture, NULL, &rect);
	rect.y = 1;
	SDL_RenderCopy(renderer, outlineTexture, NULL, &rect);
	rect.y = 0;

	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), textColor, wrapLength);
	if (textSurface == NULL) {
		RRW_LogError("unable to make text surface in text: \n" + text);
		SDL_FreeSurface(outlineSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(renderer, oldTarget);
		return finalTexture;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (textTexture == NULL) {
		RRW_LogError("unable to make real text in text: \n" + text);
		SDL_FreeSurface(outlineSurface);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(renderer, oldTarget);
		return finalTexture;
	}

	SDL_RenderCopy(renderer, textTexture, NULL, &rect);

	SDL_FreeSurface(outlineSurface);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(outlineTexture);
	SDL_DestroyTexture(textTexture);
	SDL_SetRenderTarget(renderer, oldTarget);
	return finalTexture;
}

static bool RRW_CheckMousePositionOnObject(int xMouse, int yMouse, SDL_Rect* objectPosition) 
{
	if ( xMouse >= objectPosition->x && xMouse <= objectPosition->x + objectPosition->w && 
		 yMouse >= objectPosition->y && yMouse <= objectPosition->y + objectPosition->h )
			return true;
	return false;
}

#endif // SHARED_RESOURCE_UTIL_H
