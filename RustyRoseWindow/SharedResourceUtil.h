#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef SHARED_RESOURCE_UTIL_H
#define SHARED_RESOURCE_UTIL_H

enum FontSize{
    Small,
    Medium,
    Large
};

struct Fonts {
    Fonts(TTF_Font* small, TTF_Font* medium, TTF_Font* large) {
        this->Small = small;
        this->Medium = medium;
        this->Large = large;
    }

    TTF_Font* Small = NULL;
    TTF_Font* Medium = NULL;
    TTF_Font* Large = NULL;
};

struct ScreenSize {
    ScreenSize(int width, int height) {
        this->Width = width;
        this->Height = height;
    }

    int Width;
    int Height;
};

struct ImageText {
	std::string text;
	SDL_Texture* texture = NULL;
	SDL_Rect* rect = new SDL_Rect;

	~ImageText() {
		if (this->rect) delete this->rect;
		if (this->texture) SDL_DestroyTexture(this->texture);
	}
};

struct Image {
	std::string path;
	SDL_Texture* texture = NULL;
	SDL_Rect* rect = NULL;
};

static SDL_Texture* makeTextureFromText(std::string text, SDL_Rect* size, TTF_Font* font, SDL_Color textColor, SDL_Color outlineColor, SDL_Renderer* renderer, int wrapLength)
{
	auto renderTarget = SDL_GetRenderTarget(renderer);

	SDL_Surface* outlineSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), outlineColor, wrapLength);
	if (outlineSurface == NULL) {
		printf("unable to make outline surface in text: %s\n", text.c_str());
		return nullptr;
	}

	SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(renderer, outlineSurface);
	if (outlineTexture == NULL) {
		printf("unable to make texture from outline surface in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		return nullptr;
	}

	SDL_Texture* finalTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, outlineSurface->w, outlineSurface->h);
	if (finalTexture == NULL) {
		printf("unable to make final texture in text: %s\n", text.c_str());
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

	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), textColor, wrapLength);
	if (textSurface == NULL) {
		printf("unable to make text surface in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(renderer, NULL);
		return finalTexture;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (textTexture == NULL) {
		printf("unable to make real text in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(renderer, NULL);
		return finalTexture;
	}

	SDL_RenderCopy(renderer, textTexture, NULL, &rect);

	SDL_FreeSurface(outlineSurface);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(outlineTexture);
	SDL_DestroyTexture(textTexture);
	SDL_SetRenderTarget(renderer, renderTarget);
	return finalTexture;
}

#endif // SHARED_RESOURCE_UTIL_H
