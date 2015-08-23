#include "game.h"

#include <string>

#include <SDL.h>

#include "sdl_utils.h"

using namespace std;
using namespace sdl;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sdl::Game::Game()
	: _ok(false), _window(nullptr, SDL_DestroyWindow),
	_renderer(nullptr, SDL_DestroyRenderer), _character(nullptr, SDL_DestroyTexture)
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

	_ok = true;
}


sdl::Game::~Game()
{
	IMG_Quit();
	SDL_Quit();
}


bool sdl::Game::init()
{
	_window = make_sdl_ptr(SDL_CreateWindow("My Game", 100, 100, _state.screen_width, _state.screen_height, SDL_WINDOW_SHOWN));
	if (_window == nullptr)
	{
		log::error("SDL_CreateWindow");
		return false;
	}

	_renderer = make_sdl_ptr(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (_renderer == nullptr)
	{
		log::error("SDL_CreateRenderer");
		return false;
	}

	string resPath = getResourcePath("Lesson5");
	_character = make_sdl_ptr(IMG_LoadTexture(_renderer.get(), string(resPath + "image.png").c_str()));
	if (_character == nullptr)
	{
		log::error("IMG_LoadTexture");
		return false;
	}

	if (!_init_char())
	{
		log::error("_init_char");
		return false;
	}

	return true;
}


bool sdl::Game::run()
{
	while (!_state.quit)
	{
		_check_events();
		_check_inputs();
		_render();
	}

	return true;
}


bool sdl::Game::_init_char()
{
	int w, h;
	SDL_QueryTexture(_character.get(), nullptr, nullptr, &w, &h);

	const int sw = 100, sh = 100;
	const int columns = w / sw;
	const int rows = h / sh;
	_char_clips.reserve(columns * rows);
	
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			SDL_Rect rect;
			rect.x = j * sw;
			rect.y = i * sh;
			rect.w = sw;
			rect.h = sh;
			_char_clips.push_back(rect);
		}
	}

	assert(_char_clips.size() >= 4);
	return true;
}


void sdl::Game::_check_events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) _state.quit = true;
	}
}


void sdl::Game::_check_inputs()
{
	_check_keyboard_inputs();
	_check_mouse_inputs();
	_check_joystick_inputs();
}


void sdl::Game::_check_keyboard_inputs()
{
	auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE]) _state.quit = true;

	if (state[SDL_SCANCODE_KP_4]) _state.curr_clip = 0;
	if (state[SDL_SCANCODE_KP_5]) _state.curr_clip = 1;
	if (state[SDL_SCANCODE_KP_7]) _state.curr_clip = 2;
	if (state[SDL_SCANCODE_KP_8]) _state.curr_clip = 3;

	if (state[SDL_SCANCODE_A])
	{
		_state.curr_x -= 10;
		if (_state.curr_x < 0) _state.curr_x = 0;
	}
	if (state[SDL_SCANCODE_D])
	{
		_state.curr_x += 10;
		if (_state.curr_x > _state.screen_width) _state.curr_x = _state.screen_width;
	}
	if (state[SDL_SCANCODE_W])
	{
		_state.curr_y -= 10;
		if (_state.curr_y < 0) _state.curr_y = 0;
	}
	if (state[SDL_SCANCODE_S])
	{
		_state.curr_y += 10;
		if (_state.curr_y > _state.screen_height) _state.curr_y = _state.screen_height;
	}
}


void sdl::Game::_check_mouse_inputs()
{}


void sdl::Game::_check_joystick_inputs()
{}


void sdl::Game::_render()
{
	SDL_RenderClear(_renderer.get());

	if (_state.curr_clip >= 0)
		renderTexture(_renderer, _character, &_char_clips[_state.curr_clip], _state.curr_x, _state.curr_y);

	SDL_RenderPresent(_renderer.get());
}


void sdl::Game::_render_tiled_background(const SDL_RendererPtr & ren, const SDL_TexturePtr & tex, int w, int h)
{
	SDL_Rect dst;
	if (w <= 0 || h <= 0)
	{
		int _w, _h;
		SDL_QueryTexture(tex.get(), nullptr, nullptr, &_w, &_h);
		if (w <= 0) w = _w;
		if (h <= 0) h = _h;
	}

	dst.w = w;
	dst.h = h;

	for (int x = 0; x < _state.screen_width; x += w)
	{
		for (int y = 0; y < _state.screen_height; y += h)
		{
			dst.x = x;
			dst.y = y;
			SDL_RenderCopy(ren.get(), tex.get(), nullptr, &dst);
		}
	}
}
