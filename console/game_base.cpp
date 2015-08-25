#include "game_base.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "sdl_log.h"

using namespace std;
using namespace sdl;


sdl::GameBase::GameBase()
	: _ok(false)
{
	log::init();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		log::error("SDL_Init");
		return;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		log::error("IMG_Init");
		return;
	}

	if (TTF_Init() != 0)
	{
		log::error("TTF_Init");
		return;
	}

	_ok = true;
}


sdl::GameBase::~GameBase()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
