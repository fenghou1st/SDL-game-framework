#include "game.h"

#include <string>
#include <chrono>
#include <thread>

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
	: _ok(false), _window(nullptr, SDL_DestroyWindow), _renderer(nullptr, SDL_DestroyRenderer),
	_font(nullptr, TTF_CloseFont), _character(nullptr, SDL_DestroyTexture)
{
	if (!GameBase::is_ok())
	{
		log::error("GameBase::is_ok");
		return;
	}

	_window = make_sdl_ptr(SDL_CreateWindow(GAME_TITLE.c_str(), 100, 100, _state.screen_width, _state.screen_height, SDL_WINDOW_SHOWN));
	if (_window == nullptr)
	{
		log::error("SDL_CreateWindow");
		return;
	}

	_renderer = make_sdl_ptr(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED));
	if (_renderer == nullptr)
	{
		log::error("SDL_CreateRenderer");
		return;
	}

	string images_path = get_resource_path("images");
	_character = make_sdl_ptr(IMG_LoadTexture(_renderer.get(), string(images_path + CHAR_FILE).c_str()));
	if (_character == nullptr)
	{
		log::error("IMG_LoadTexture");
		return;
	}

	string fonts_path = get_resource_path("fonts");
	_font = make_sdl_ptr(TTF_OpenFont((fonts_path + FONT_FILE).c_str(), FONT_SIZE));
	if (_font == nullptr)
	{
		log::error("TTF_OpenFont");
		return;
	}

	if (!_init_char())
	{
		log::error("_init_char");
		return;
	}

	_ok = true;
}


sdl::Game::~Game()
{}


bool sdl::Game::run()
{
	typedef chrono::time_point<chrono::steady_clock, chrono::nanoseconds> time_point_nano;

	uint64_t nano_seconds_per_frame;
	chrono::nanoseconds duration_per_frame;
	time_point_nano frame_end;
	if (FRAME_RATE > 0)
	{
		nano_seconds_per_frame = 1000000000 / FRAME_RATE;
		duration_per_frame = chrono::nanoseconds(nano_seconds_per_frame);
		frame_end = chrono::steady_clock::now() + duration_per_frame;
	}

	while (!_state.quit)
	{
		_check_events();
		_check_inputs();
		_render();

		++_state.frame_count;
		if (FRAME_RATE > 0)
		{
			while (chrono::steady_clock::now() < frame_end) this_thread::sleep_for(1ms);
			frame_end += duration_per_frame;
		}
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

	// tips
	string tips = "A, D, S, W: move;  NumPad 4, 5, 7, 8: change color;  ESC: quit";
	auto text_tips = render_text(_renderer, tips, _font, FONT_COLOR);
	render_texture(_renderer, text_tips, _state.screen_width / 2, _state.screen_height / 2);

	// frame rate
	static auto last_time = chrono::steady_clock::now();
	static auto last_frame_count = _state.frame_count;
	static char frame_rate_buf[100] = "0";
	auto curr_time = chrono::steady_clock::now();
	if (last_time + chrono::seconds(1) <= curr_time)
	{
		auto curr_frame_count = _state.frame_count;
		chrono::duration<double> elapsed_seconds = curr_time - last_time;
		auto frame_rate = (curr_frame_count - last_frame_count) / elapsed_seconds.count();
		sprintf(frame_rate_buf, "%.2f", frame_rate);

		last_time = curr_time;
		last_frame_count = curr_frame_count;
	}
	auto text_frame_rate = render_text(_renderer, frame_rate_buf, _font, FONT_COLOR);
	render_texture(_renderer, text_frame_rate, 0, _state.screen_height - 1, TEXTURE_ORIGIN::BOTTOM_LEFT);

	// character
	if (_state.curr_clip >= 0)
		render_texture(_renderer, _character, &_char_clips[_state.curr_clip], _state.curr_x, _state.curr_y);

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
