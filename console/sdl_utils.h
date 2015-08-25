#pragma once

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "sdl_log.h"
#include "sdl_ptr.h"

namespace sdl
{
	enum TEXTURE_ORIGIN
	{
		TOP_LEFT		= 0,
		TOP_CENTER		= 1,
		TOP_RIGHT		= 2,
		CENTER_LEFT		= 3,
		CENTER			= 4,
		CENTER_RIGHT	= 5,
		BOTTOM_LEFT		= 6,
		BOTTOM_CENTER	= 7,
		BOTTOM_RIGHT	= 8,
	};


	/*
	* Get the resource path for resources located in res/subDir
	* It's assumed the project directory is structured like:
	* bin/
	*  the executable
	* res/
	*  Lesson1/
	*  Lesson2/
	*
	* Paths returned will be Lessons/res/subDir
	*/
	std::string get_resource_path(const std::string & subDir = "")
	{
		static std::string baseRes;

#ifdef _WIN32
		const char PATH_SEP = '\\';
#else
		const char PATH_SEP = '/';
#endif

		if (baseRes.empty())
		{
			char *basePath = SDL_GetBasePath();
			if (basePath)
			{
				baseRes = basePath;
				SDL_free(basePath);
			}
			else
			{
				log::error("failed to get resource path");
				return "";
			}

			size_t pos = baseRes.rfind("bin");
			baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
		}

		return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
	}


	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving the texture's width and height
	* @param x  The x coordinate to draw to
	* @param y  The y coordinate to draw to
	*/
	void render_texture(const SDL_RendererPtr & ren, const SDL_TexturePtr & tex, const SDL_Rect * clip,
		int x, int y, TEXTURE_ORIGIN texture_origin = TEXTURE_ORIGIN::CENTER)
	{
		SDL_Rect dst;
		if (clip != nullptr)
		{
			dst.w = clip->w;
			dst.h = clip->h;
		}
		else
			SDL_QueryTexture(tex.get(), nullptr, nullptr, &dst.w, &dst.h);

		switch (texture_origin)
		{
		case TOP_LEFT:
			dst.x = x;
			dst.y = y;
			break;
		case TOP_CENTER:
			dst.x = x - dst.w / 2;
			dst.y = y;
			break;
		case TOP_RIGHT:
			dst.x = x - dst.w;
			dst.y = y;
			break;
		case CENTER_LEFT:
			dst.x = x;
			dst.y = y - dst.h / 2;
			break;
		case CENTER_RIGHT:
			dst.x = x - dst.w;
			dst.y = y - dst.h / 2;
			break;
		case BOTTOM_LEFT:
			dst.x = x;
			dst.y = y - dst.h;
			break;
		case BOTTOM_CENTER:
			dst.x = x - dst.w / 2;
			dst.y = y - dst.h;
			break;
		case BOTTOM_RIGHT:
			dst.x = x - dst.w;
			dst.y = y - dst.h;
			break;
		case CENTER:
		default:
			dst.x = x - dst.w / 2;
			dst.y = y - dst.h / 2;
		}

		SDL_RenderCopy(ren.get(), tex.get(), clip, &dst);
	}


	void render_texture(const SDL_RendererPtr & ren, const SDL_TexturePtr & tex,
		int x, int y, TEXTURE_ORIGIN texture_origin = TEXTURE_ORIGIN::CENTER)
	{
		render_texture(ren, tex, nullptr, x, y, texture_origin);
	}


	SDL_TexturePtr render_text(const SDL_RendererPtr & ren, const std::string & message,
		const SDL_FontPtr & font, SDL_Color color)
	{
		SDL_TexturePtr tex(nullptr, SDL_DestroyTexture);

		auto surf = make_sdl_ptr(TTF_RenderUTF8_Blended(font.get(), message.c_str(), color));
		if (surf == nullptr)
		{
			log::error("TTF_RenderText");
			return tex;
		}

		tex = make_sdl_ptr(SDL_CreateTextureFromSurface(ren.get(), surf.get()));
		if (tex == nullptr)
		{
			log::error("SDL_CreateTextureFromSurface");
			return tex;
		}

		return tex;
	}
}
