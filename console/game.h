#pragma once

#include <cstdio>
#include <cassert>
#include <vector>

#include "common/common.h"

#include "sdl_log.h"
#include "sdl_ptr.h"


namespace sdl
{
	using namespace std;

	const int INIT_SCREEN_WIDTH = 1600;
	const int INIT_SCREEN_HEIGHT = 900;
	const int TILE_SIZE = 64;


	class State
	{
	public:
		State() :
			screen_width(INIT_SCREEN_WIDTH),
			screen_height(INIT_SCREEN_HEIGHT),
			quit(false),
			curr_clip(-1),
			curr_x(INIT_SCREEN_WIDTH / 2),
			curr_y(INIT_SCREEN_HEIGHT / 2)
		{}

		int screen_width;
		int screen_height;
		bool quit;
		int curr_clip;
		int curr_x;
		int curr_y;
	};


	class Game
	{
	public:
		Game();
		~Game();

		bool is_ok() const { return _ok; };
		bool init();
		bool run();

	private:
		bool _init_char();
		void _check_events();
		void _check_inputs();
		void _check_keyboard_inputs();
		void _check_mouse_inputs();
		void _check_joystick_inputs();
		void _render();

		void _render_tiled_background(const SDL_RendererPtr & ren, const SDL_TexturePtr & tex, int w = 0, int h = 0);

		bool _ok;
		State _state;
		SDL_WindowPtr _window;
		SDL_RendererPtr _renderer;
		SDL_TexturePtr _character;
		vector<SDL_Rect> _char_clips;
	};
}
