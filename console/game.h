#pragma once

#include <cstdio>
#include <cassert>
#include <vector>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <fstream>

#define SI_SUPPORT_IOSTREAMS
#include "simple_ini/SimpleIni.h"

#include "game/common.h"

#include "sdl_log.h"
#include "sdl_ptr.h"
#include "game_base.h"


namespace sdl
{
	using namespace std;

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


	class Config
	{
	public:
		int screen_width;
		int screen_height;
		int frame_rate;
	};


	class State
	{
	public:
		State() : quit(false), frame_count(0) {}

		bool quit;
		uint64_t frame_count;
		int curr_color;
		int curr_x;
		int curr_y;
	};


	class Game : public GameBase
	{
	public:
		Game();
		~Game();

		bool is_ok() const { return _ok; };
		bool run();

	private:
		bool _init_pref_path();
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

		void _render_tiled_background(const SDL_RendererPtr & ren, const SDL_TexturePtr & tex, int w = 0, int h = 0);

		bool _ok;
		string _pref_path;
		CSimpleIniA _ini;
		Config _config;
		State _state;
		SDL_WindowPtr _window;
		SDL_RendererPtr _renderer;
		SDL_FontPtr _font;
		SDL_TexturePtr _character;
		vector<SDL_Rect> _char_clips;
	};
}
