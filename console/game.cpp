#include "game.h"

#include <string>
#include <chrono>
#include <thread>
#include <fstream>

#include <boost/filesystem.hpp>

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
	: _ok(false), _ini(true),
	_window(nullptr, SDL_DestroyWindow), _renderer(nullptr, SDL_DestroyRenderer),
	_font(nullptr, TTF_CloseFont), _character(nullptr, SDL_DestroyTexture)
{
	if (!GameBase::is_ok())			{ log::error("GameBase::is_ok");				return; }

	if (!_init_pref_path())			{ log::error("Game::_init_pref_path");			return; }

	if (!_load_config())			{ log::error("Game::_load_config");				return; }

	if (!_load_state())				{ log::error("Game::_load_state");				return; }

	if (!_create_basic_objects())	{ log::error("Game::_create_basic_objects");	return; }

	if (!_load_resources())			{ log::error("Game::_load_resources");			return; }

	if (!_init_char())				{ log::error("Game::_init_char");				return; }

	_ok = true;
}


sdl::Game::~Game()
{
	_save_state();
	_save_config();
}


bool sdl::Game::run()
{
	typedef chrono::time_point<chrono::steady_clock, chrono::nanoseconds> time_point_nano;

	uint64_t nano_seconds_per_frame;
	chrono::nanoseconds duration_per_frame;
	time_point_nano frame_end;
	if (_config.frame_rate > 0)
	{
		nano_seconds_per_frame = 1000000000 / _config.frame_rate;
		duration_per_frame = chrono::nanoseconds(nano_seconds_per_frame);
		frame_end = chrono::steady_clock::now() + duration_per_frame;
	}

	while (!_state.quit)
	{
		_check_events();
		_check_inputs();
		_render();

		++_state.frame_count;
		if (_config.frame_rate > 0)
		{
			while (chrono::steady_clock::now() < frame_end) this_thread::sleep_for(1ms);
			frame_end += duration_per_frame;
		}
	}

	return true;
}


bool sdl::Game::_init_pref_path()
{
	auto base_path = SDL_GetPrefPath(ORG_NAME.c_str(), APP_NAME.c_str());
	if (base_path == nullptr)
	{
		log::error("SDL_GetPrefPath");
		return false;
	}
	_pref_path = base_path;
	SDL_free(base_path);

	if (!boost::filesystem::exists(_pref_path) && !boost::filesystem::create_directories(_pref_path))
	{
		log::error("create_directories");
		return false;
	}

	return true;
}


bool sdl::Game::_load_config()
{
	string ini_file = _pref_path + INI_FILE;
	if (!boost::filesystem::exists(ini_file)) _ini.SaveFile(ini_file.c_str());
	SI_Error rc = _ini.LoadFile(ini_file.c_str());
	if (rc < 0) return false;

	_config.screen_width = _ini.GetLongValue("System", "Screen Width", 1600);
	_config.screen_height = _ini.GetLongValue("System", "Screen Height", 900);
	_config.frame_rate = _ini.GetLongValue("System", "Frame Rate", 60);

	return true;
}


bool sdl::Game::_save_config()
{
	string ini_file = _pref_path + INI_FILE;

	_ini.SetLongValue("System", "Screen Width", _config.screen_width);
	_ini.SetLongValue("System", "Screen Height", _config.screen_height);
	_ini.SetLongValue("System", "Frame Rate", _config.frame_rate);

	auto rc = _ini.SaveFile(ini_file.c_str());
	return rc >= 0;
}


bool sdl::Game::_load_state()
{
	string save_file = _pref_path + SAVE_FILE;
	
	if (!boost::filesystem::exists(save_file) || boost::filesystem::file_size(save_file) <= 0)
		return _load_default_state();

	ifstream state(save_file, ios::binary);
	char buf[8];

	state.read(buf, 8);
	if (memcmp(SAVE_FILE_TAG, buf, 8) != 0)
	{
		log::error("Incompatible save file");
		return _load_default_state();
	}

	state.read(buf, sizeof(int));
	_state.curr_color = *reinterpret_cast<int *>(buf);

	state.read(buf, sizeof(int));
	_state.curr_x = *reinterpret_cast<int *>(buf);

	state.read(buf, sizeof(int));
	_state.curr_y = *reinterpret_cast<int *>(buf);

	return true;
}


bool sdl::Game::_load_default_state()
{
	_state.curr_color = -1;
	_state.curr_x = _config.screen_width / 2;
	_state.curr_y = _config.screen_height / 2;

	return true;
}


bool sdl::Game::_save_state()
{
	string save_file = _pref_path + SAVE_FILE;
	ofstream state(save_file, ios::binary | ios::trunc);

	state.write(SAVE_FILE_TAG, 8);
	state.write(reinterpret_cast<char *>(&_state.curr_color), sizeof(int));
	state.write(reinterpret_cast<char *>(&_state.curr_x), sizeof(int));
	state.write(reinterpret_cast<char *>(&_state.curr_y), sizeof(int));

	return true;
}


bool sdl::Game::_create_basic_objects()
{
	_window = make_sdl_ptr(SDL_CreateWindow(APP_NAME.c_str(), 100, 100, _config.screen_width, _config.screen_height, SDL_WINDOW_SHOWN));
	if (_window == nullptr)
	{
		log::error("SDL_CreateWindow");
		return false;
	}

	_renderer = make_sdl_ptr(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED));
	if (_renderer == nullptr)
	{
		log::error("SDL_CreateRenderer");
		return false;
	}

	return true;
}


bool sdl::Game::_load_resources()
{
	string images_path = get_resource_path("images");
	_character = make_sdl_ptr(IMG_LoadTexture(_renderer.get(), string(images_path + CHAR_FILE).c_str()));
	if (_character == nullptr)
	{
		log::error("IMG_LoadTexture");
		return false;
	}

	string fonts_path = get_resource_path("fonts");
	_font = make_sdl_ptr(TTF_OpenFont((fonts_path + FONT_FILE).c_str(), FONT_SIZE));
	if (_font == nullptr)
	{
		log::error("TTF_OpenFont");
		return false;
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

	if (state[SDL_SCANCODE_KP_4]) _state.curr_color = 0;
	if (state[SDL_SCANCODE_KP_5]) _state.curr_color = 1;
	if (state[SDL_SCANCODE_KP_7]) _state.curr_color = 2;
	if (state[SDL_SCANCODE_KP_8]) _state.curr_color = 3;

	if (state[SDL_SCANCODE_A])
	{
		_state.curr_x -= 10;
		if (_state.curr_x < 0) _state.curr_x = 0;
	}
	if (state[SDL_SCANCODE_D])
	{
		_state.curr_x += 10;
		if (_state.curr_x > _config.screen_width) _state.curr_x = _config.screen_width;
	}
	if (state[SDL_SCANCODE_W])
	{
		_state.curr_y -= 10;
		if (_state.curr_y < 0) _state.curr_y = 0;
	}
	if (state[SDL_SCANCODE_S])
	{
		_state.curr_y += 10;
		if (_state.curr_y > _config.screen_height) _state.curr_y = _config.screen_height;
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
	render_texture(_renderer, text_tips, _config.screen_width / 2, _config.screen_height / 2);

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
	render_texture(_renderer, text_frame_rate, 0, _config.screen_height - 1, TEXTURE_ORIGIN::BOTTOM_LEFT);

	// character
	if (_state.curr_color >= 0)
		render_texture(_renderer, _character, &_char_clips[_state.curr_color], _state.curr_x, _state.curr_y);

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

	for (int x = 0; x < _config.screen_width; x += w)
	{
		for (int y = 0; y < _config.screen_height; y += h)
		{
			dst.x = x;
			dst.y = y;
			SDL_RenderCopy(ren.get(), tex.get(), nullptr, &dst);
		}
	}
}
