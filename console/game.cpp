#include "game.h"

#ifndef _WIN32
#include <unistd.h>
#endif
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

#include <boost/filesystem.hpp>

#include <SDL.h>

#define SI_SUPPORT_IOSTREAMS
#include "simple_ini/SimpleIni.h"

#include "game/data_file_helper.h"

#include "sdl_log.h"
#include "sdl_utils.h"
#include "sdl_ptr.h"
#include "game_base.h"

using namespace std;
using namespace fenghou;
using namespace fenghou::game;
using namespace sdl;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////

namespace sdl
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Config
	{
	public:
		int screen_width;
		int screen_height;
		int frame_rate;
		string screen_tips;
	};


	class State
	{
	public:
		State() : quit(false), frame_count(0) {}

		bool quit;
		uint64_t frame_count;
		int_fast8_t curr_color;
		int_fast16_t curr_x;
		int_fast16_t curr_y;
		string screen_tips;
	};


	class Game::Impl : public GameBase
	{
	public:
		Impl();
		~Impl();

		bool is_ok() const { return _ok; }
		bool run();

	private:
		bool _load_config();
		bool _save_config();
		bool _load_state();
		bool _load_default_state();
		bool _save_state();
		bool _create_basic_objects();
		bool _load_resources();
		bool _init_char();
		void _check_events();
		void _check_inputs();
		void _check_keyboard_inputs();
		void _check_mouse_inputs();
		void _check_joystick_inputs();
		void _render();

		bool _ok;
		CSimpleIniA _ini;
		Config _config;
		State _state;
		SDL_WindowPtr _window;
		SDL_RendererPtr _renderer;
		SDL_FontPtr _font;
		SDL_TexturePtr _character;
		vector<SDL_Rect> _char_clips;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}


// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////

	// When not sepecify tile width/height, it will use the actual width/height of the tile
	void render_tiled_background(const SDL_RendererPtr & ren, const SDL_TexturePtr & tex,
		int screen_width, int screen_height, int tile_w = 0, int tile_h = 0)
	{
		SDL_Rect dst;
		if (tile_w <= 0 || tile_h <= 0)
		{
			int _w, _h;
			SDL_QueryTexture(tex.get(), nullptr, nullptr, &_w, &_h);
			if (tile_w <= 0) tile_w = _w;
			if (tile_h <= 0) tile_h = _h;
		}

		dst.w = tile_w;
		dst.h = tile_h;

		for (int x = 0; x < screen_width; x += tile_w)
		{
			for (int y = 0; y < screen_height; y += tile_h)
			{
				dst.x = x;
				dst.y = y;
				SDL_RenderCopy(ren.get(), tex.get(), nullptr, &dst);
			}
		}
	}

	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////

	const string ORG_NAME = "Kazesoft";
	const string APP_NAME = "My Game";
	const string INI_FILE = "game.ini";
	const string SAVE_FILE = "save.dat";
	const char SAVE_FILE_TAG[8] = { 'S', 'A', 'V', '.' , '0' , '0' , '0' , '1' };
	const string CHAR_FILE = "character.png";
	const string FONT_FILE = "msyh.ttc";
	const int TILE_SIZE = 64;
	const int FONT_SIZE = 36;
	const SDL_Color FONT_COLOR = { 255, 255, 255, 255 };
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

//// Game //////////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game() : impl(new Impl) {}


Game::~Game() {}


bool Game::is_ok() const { return impl->is_ok(); }


bool Game::run() { return impl->run(); }

//// Game::Impl ////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Impl::Impl()
	: GameBase(ORG_NAME, APP_NAME), _ok(false), _ini(true),
	_window(nullptr, SDL_DestroyWindow), _renderer(nullptr, SDL_DestroyRenderer),
	_font(nullptr, TTF_CloseFont), _character(nullptr, SDL_DestroyTexture)
{
	if (!GameBase::is_ok())			{ log::error("GameBase::is_ok");				return; }

	if (!_load_config())			{ log::error("Game::_load_config");				return; }

	if (!_load_state())				{ log::error("Game::_load_state");				return; }

	if (!_create_basic_objects())	{ log::error("Game::_create_basic_objects");	return; }

	if (!_load_resources())			{ log::error("Game::_load_resources");			return; }

	if (!_init_char())				{ log::error("Game::_init_char");				return; }

	_ok = true;
}


Game::Impl::~Impl()
{
	_save_state();
	_save_config();
}


bool Game::Impl::run()
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


bool Game::Impl::_load_config()
{
	string ini_file = GameBase::get_pref_path() + INI_FILE;
	if (!boost::filesystem::exists(ini_file)) _ini.SaveFile(ini_file.c_str());
	SI_Error rc = _ini.LoadFile(ini_file.c_str());
	if (rc < 0) return false;

	_config.screen_width = _ini.GetLongValue("System", "Screen Width", 1600);
	_config.screen_height = _ini.GetLongValue("System", "Screen Height", 900);
	_config.frame_rate = _ini.GetLongValue("System", "Frame Rate", 60);
	_config.screen_tips = _ini.GetValue("System", "Screen Tips");

	return true;
}


bool Game::Impl::_save_config()
{
	string ini_file = GameBase::get_pref_path() + INI_FILE;

	_ini.SetLongValue("System", "Screen Width", _config.screen_width);
	_ini.SetLongValue("System", "Screen Height", _config.screen_height);
	_ini.SetLongValue("System", "Frame Rate", _config.frame_rate);
	_ini.SetValue("System", "Screen Tips", _config.screen_tips.c_str());

	auto rc = _ini.SaveFile(ini_file.c_str());
	return rc >= 0;
}


bool Game::Impl::_load_state()
{
	string save_file = GameBase::get_pref_path() + SAVE_FILE;
	
	if (!boost::filesystem::exists(save_file) || boost::filesystem::file_size(save_file) <= 0)
		return _load_default_state();

	ifstream state(save_file, ios::binary);

	char file_tag[sizeof(SAVE_FILE_TAG)];
	data_file_helper::read(state, file_tag);
	if (memcmp(SAVE_FILE_TAG, file_tag, 8) != 0)
	{
		log::error("Incompatible save file");
		return _load_default_state();
	}

	data_file_helper::read(state, _state.curr_color);
	data_file_helper::read(state, _state.curr_x);
	data_file_helper::read(state, _state.curr_y);
	data_file_helper::read(state, _state.screen_tips);

	return true;
}


// TODO: load data from platform independent format, such as json
bool Game::Impl::_load_default_state()
{
	_state.curr_color = -1;
	_state.curr_x = _config.screen_width / 2;
	_state.curr_y = _config.screen_height / 2;
	_state.screen_tips = "A, D, S, W: move;  NumPad 4, 5, 7, 8: change color;  ESC: quit";

	return true;
}


bool Game::Impl::_save_state()
{
	string save_file = GameBase::get_pref_path() + SAVE_FILE;
	ofstream state(save_file, ios::binary | ios::trunc);

	data_file_helper::write(state, SAVE_FILE_TAG);
	data_file_helper::write(state, _state.curr_color);
	data_file_helper::write(state, _state.curr_x);
	data_file_helper::write(state, _state.curr_y);
	data_file_helper::write(state, _state.screen_tips);

	return true;
}


bool Game::Impl::_create_basic_objects()
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


bool Game::Impl::_load_resources()
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


bool Game::Impl::_init_char()
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


void Game::Impl::_check_events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) _state.quit = true;
	}
}


void Game::Impl::_check_inputs()
{
	_check_keyboard_inputs();
	_check_mouse_inputs();
	_check_joystick_inputs();
}


void Game::Impl::_check_keyboard_inputs()
{
	auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE]) _state.quit = true;

	if (state[SDL_SCANCODE_KP_4]) _state.curr_color = 0;
	if (state[SDL_SCANCODE_KP_5]) _state.curr_color = 1;
	if (state[SDL_SCANCODE_KP_7]) _state.curr_color = 2;
	if (state[SDL_SCANCODE_KP_8]) _state.curr_color = 3;

	if (state[SDL_SCANCODE_A]) _state.curr_x -= 10;
	if (state[SDL_SCANCODE_D]) _state.curr_x += 10;
	if (state[SDL_SCANCODE_W]) _state.curr_y -= 10;
	if (state[SDL_SCANCODE_S]) _state.curr_y += 10;

	if (_state.curr_x < 0) _state.curr_x = 0;
	else if (_state.curr_x > _config.screen_width) _state.curr_x = _config.screen_width;

	if (_state.curr_y < 0) _state.curr_y = 0;
	else if (_state.curr_y > _config.screen_height) _state.curr_y = _config.screen_height;
}


void Game::Impl::_check_mouse_inputs()
{}


void Game::Impl::_check_joystick_inputs()
{}


void Game::Impl::_render()
{
	SDL_RenderClear(_renderer.get());

	// tips
	auto text_tips = render_text(_renderer, _config.screen_tips, _font, FONT_COLOR);
	render_texture(_renderer, text_tips, _config.screen_width / 2, _config.screen_height / 2);

	// frame rate
	static auto last_time = chrono::steady_clock::now();
	static auto last_frame_count = _state.frame_count;
	static char frame_rate_buf[10] = "0";
	auto curr_time = chrono::steady_clock::now();
	if (last_time + chrono::seconds(1) <= curr_time)
	{
		auto curr_frame_count = _state.frame_count;
		chrono::duration<double> elapsed_seconds = curr_time - last_time;
		auto frame_rate = (curr_frame_count - last_frame_count) / elapsed_seconds.count();
		snprintf(frame_rate_buf, sizeof(frame_rate_buf), "%.2f", frame_rate);

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
