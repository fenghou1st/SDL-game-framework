#include "game.h"

#include <string>
#include <chrono>
#include <thread>
#include <vector>

#include <SDL.h>

#include "game/sdl/sdl_log.h"
#include "game/sdl/sdl_utils.h"
#include "game/sdl/sdl_ptr.h"

#include "gui/cegui.h"

#include "game_base.h"
#include "config.h"
#include "state.h"

using namespace std;
using namespace fenghou;
using namespace sdl;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////

namespace sdl
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Game::Impl : public GameBase
	{
	public:
		Impl();
		~Impl();

		bool is_ok() const { return _ok; }
		bool run();

	private:
		bool _load_config();
		bool _load_state();
		bool _create_basic_objects();
		bool _load_resources();
		bool _init_char();
		void _check_events();
		void _check_inputs();
		void _check_keyboard_inputs();
		void _check_mouse_inputs();
		void _check_joystick_inputs();
		void _update_state();
		void _render(float time_elapsed);

		bool _ok;
		Config _config;
		State _state;
		SDL_WindowPtr _window;
		SDL_RendererPtr _renderer;
		SDL_FontPtr _font;
		SDL_TexturePtr _character;
		SDL_GLContext _context;
		CEGUI::OpenGL3Renderer * _gl_renderer;
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
	const string SAVE_BIN_FILE = "save.dat";
	const string SAVE_TXT_FILE = "save.txt";
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
	: GameBase(ORG_NAME, APP_NAME), _ok(false),
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
	CEGUI::System::destroy();
	CEGUI::OpenGL3Renderer::destroy(*_gl_renderer);
	_gl_renderer = nullptr;

	if (_context) SDL_GL_DeleteContext(_context);
}


bool Game::Impl::run()
{
	typedef chrono::time_point<chrono::steady_clock, chrono::nanoseconds> time_point_nano;

	uint64_t nano_seconds_per_frame;
	chrono::nanoseconds duration_per_frame;
	time_point_nano frame_begin, frame_end;

	frame_begin = chrono::steady_clock::now();
	if (_config.frame_rate > 0)
	{
		nano_seconds_per_frame = 1000000000 / _config.frame_rate;
		duration_per_frame = chrono::nanoseconds(nano_seconds_per_frame);
		frame_end = frame_begin + duration_per_frame;
	}

	while (!_state.quit)
	{
		time_point_nano curr_begin = chrono::steady_clock::now();
		chrono::duration<float, std::milli> time_elapsed = curr_begin - frame_begin;
		frame_begin = curr_begin;

		_check_events();
		_check_inputs();
		_update_state();
		_render(time_elapsed.count());

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
	_config.set(GameBase::get_pref_path() + DIR_PREF_CFG + PATH_SEP + INI_FILE);
	return _config.load();
}


bool Game::Impl::_load_state()
{
	_state.set(GameBase::get_pref_path() + DIR_PREF_SAVE + PATH_SEP + SAVE_BIN_FILE,
		GameBase::get_data_path() + "defaults" + PATH_SEP + SAVE_TXT_FILE);
	return _state.load();
}


bool Game::Impl::_create_basic_objects()
{
	_window = make_sdl_ptr(SDL_CreateWindow(APP_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_config.screen_width, _config.screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));
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

	SDL_ShowCursor(0);

	_context = SDL_GL_CreateContext(_window.get());
	if (_context == nullptr)
	{
		log::error("SDL_GL_CreateContext");
		return false;
	}

	//
	cegui::initCEGUI(GameBase::get_data_path(), GameBase::get_pref_path() + DIR_PREF_LOG + PATH_SEP,
		_config.screen_width, _config.screen_height);

	CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(_config.screen_width, _config.screen_height));

	cegui::initWindows();

	glClearColor(0, 0, 0, 255);

	_gl_renderer = static_cast<CEGUI::OpenGL3Renderer *>(CEGUI::System::getSingleton().getRenderer());

	return true;
}


bool Game::Impl::_load_resources()
{
	auto char_path = GameBase::get_data_path() + "images" + PATH_SEP + CHAR_FILE;
	_character = make_sdl_ptr(IMG_LoadTexture(_renderer.get(), char_path.c_str()));
	if (_character == nullptr)
	{
		log::error("IMG_LoadTexture");
		return false;
	}

	auto font_path = GameBase::get_data_path() + "fonts" + PATH_SEP + FONT_FILE;
	_font = make_sdl_ptr(TTF_OpenFont(font_path.c_str(), FONT_SIZE));
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
		switch (e.type)
		{
		case SDL_QUIT:
			_state.quit = true;
			break;
		case SDL_MOUSEMOTION:
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectMousePosition(static_cast<float>(e.motion.x), static_cast<float>(e.motion.y));
			break;
		case SDL_MOUSEBUTTONDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectMouseButtonDown(cegui::SDLtoCEGUIMouseButton(e.button.button));
			break;
		case SDL_MOUSEBUTTONUP:
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectMouseButtonUp(cegui::SDLtoCEGUIMouseButton(e.button.button));
			break;
		case SDL_MOUSEWHEEL:
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectMouseWheelChange(static_cast<float>(e.wheel.y));
			break;
		case SDL_KEYDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectKeyDown(cegui::toCEGUIKey(e.key.keysym.scancode));
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectChar(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectKeyUp(cegui::toCEGUIKey(e.key.keysym.scancode));
			break;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				CEGUI::System::getSingleton().notifyDisplaySizeChanged(
					CEGUI::Sizef(static_cast<float>(e.window.data1), static_cast<float>(e.window.data2)));
				glViewport(0, 0, e.window.data1, e.window.data2);
			}
			break;
		default:
			;
		}
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


void Game::Impl::_update_state()
{}


void Game::Impl::_render(float time_elapsed)
{
	SDL_RenderClear(_renderer.get());

	// tips
	auto text_tips = render_text(_renderer, _state.screen_tips, _font, FONT_COLOR);
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glClear(GL_COLOR_BUFFER_BIT);

	CEGUI::System::getSingleton().injectTimePulse(time_elapsed);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);

	_gl_renderer->beginRendering();
	CEGUI::System::getSingleton().renderAllGUIContexts();
	_gl_renderer->endRendering();

	SDL_GL_SwapWindow(_window.get());
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SDL_RenderPresent(_renderer.get());
}
