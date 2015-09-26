#include "game.h"

#include <string>
#include <chrono>
#include <thread>
#include <vector>

#include <boost/filesystem.hpp>

#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <SDL.h>

#include "game/sdl/sdl_log.h"
#include "game/sdl/sdl_utils.h"
#include "game/sdl/sdl_ptr.h"

#include "actions/actions.h"
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
		void _update_state(float time_elapsed);
		void _render();

		bool _ok;
		Config _config;
		State _state;

		SDL_WindowPtr _window;
		SDL_RendererPtr _renderer;
		SDL_FontPtr _font;
		SDL_TexturePtr _character;
		SDL_SurfacePtr _gui_surface;

		gcn::SDLInput * _gcn_input;
		gcn::SDLGraphics * _gcn_graphics;
		gcn::SDLImageLoader * _gcn_imageLoader;
		gcn::Gui * _gcn_gui;
		gcn::Container * _gcn_top;
		gcn::ImageFont * _gcn_font;
		gcn::Label * _gcn_label;

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

	const string CHAR_FILE = "character.png";
	const string FONT_FILE = "msyh.ttc";
	const string FONT_IMG_FILE = "fixedfont.bmp";
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
	_font(nullptr, TTF_CloseFont), _character(nullptr, SDL_DestroyTexture), _gui_surface(nullptr, SDL_FreeSurface)
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
	_state.save();

	delete _gcn_label;
	delete _gcn_font;
	delete _gcn_top;
	delete _gcn_gui;
	delete _gcn_input;
	delete _gcn_graphics;
	delete _gcn_imageLoader;
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
		chrono::duration<float> time_elapsed = curr_begin - frame_begin;
		frame_begin = curr_begin;

		_check_events();
		_check_inputs();
		_update_state(time_elapsed.count());
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
	string ini_file = GameBase::get_pref_path() + DIR_PREF_CFG + PATH_SEP + INI_FILE;
	string default_ini_file = GameBase::get_data_path() + "defaults" + PATH_SEP + INI_FILE;

	if (!boost::filesystem::exists(ini_file)) boost::filesystem::copy_file(default_ini_file, ini_file);

	_config.set(ini_file);
	return _config.load();
}


bool Game::Impl::_load_state()
{
	string save_file = GameBase::get_pref_path() + DIR_PREF_SAVE + PATH_SEP + SAVE_FILE;
	string default_save_file = GameBase::get_data_path() + "defaults" + PATH_SEP + SAVE_FILE;

	if (!boost::filesystem::exists(save_file)) boost::filesystem::copy_file(default_save_file, save_file);

	_state.set(save_file);
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

	//
	_gui_surface = make_sdl_ptr(
		SDL_CreateRGBSurface(0, _config.screen_width, _config.screen_height, 32, RMASK, GMASK, BMASK, AMASK));
	if (_gui_surface == nullptr)
	{
		log::error("SDL_CreateRGBSurface");
		return false;
	}

	_gcn_imageLoader = new gcn::SDLImageLoader();
	gcn::Image::setImageLoader(_gcn_imageLoader);

	_gcn_graphics = new gcn::SDLGraphics();
	_gcn_graphics->setTarget(_gui_surface.get());

	_gcn_input = new gcn::SDLInput();

	_gcn_top = new gcn::Container();
	_gcn_top->setDimension(gcn::Rectangle(0, 0, _config.screen_width / 4, _config.screen_height / 8));

	_gcn_gui = new gcn::Gui();
	_gcn_gui->setGraphics(_gcn_graphics);
	_gcn_gui->setInput(_gcn_input);
	_gcn_gui->setTop(_gcn_top);

	auto font_path = GameBase::get_data_path() + "fonts" + PATH_SEP + FONT_IMG_FILE;
	_gcn_font = new gcn::ImageFont(font_path, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	gcn::Widget::setGlobalFont(_gcn_font);

	_gcn_label = new gcn::Label("Hello World");
	_gcn_label->setForegroundColor(gcn::Color(255, 255, 255));
	_gcn_label->setPosition(10, 10);
	_gcn_top->add(_gcn_label);

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
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			break;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {}
			break;
		default:
			;
		}

		_gcn_input->pushInput(e);
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

	//
	if (state[SDL_SCANCODE_KP_4]) _state.self->appearance = 0;
	if (state[SDL_SCANCODE_KP_5]) _state.self->appearance = 1;
	if (state[SDL_SCANCODE_KP_7]) _state.self->appearance = 2;
	if (state[SDL_SCANCODE_KP_8]) _state.self->appearance = 3;

	//
	double phi;
	double power_ratio = 1;
	const double pi = std::acos(-1);
	if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_W]) phi = pi * 3 / 4;
	else if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_S]) phi = pi * 5 / 4;
	else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D]) phi = pi * 7 / 4;
	else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_W]) phi = pi * 1 / 4;
	else if (state[SDL_SCANCODE_A]) phi = pi * 4 / 4;
	else if (state[SDL_SCANCODE_S]) phi = pi * 6 / 4;
	else if (state[SDL_SCANCODE_D]) phi = pi * 0 / 4;
	else if (state[SDL_SCANCODE_W]) phi = pi * 2 / 4;
	else power_ratio = 0;

	if (power_ratio > 0) _state.self->add_action(new Move(phi, pi / 2, power_ratio));
}


void Game::Impl::_check_mouse_inputs()
{}


void Game::Impl::_check_joystick_inputs()
{}


void Game::Impl::_update_state(float time_elapsed)
{
	for (auto & character : _state.chars)
	{
		for (auto & action : character->actions) action->apply(character, &_state);
		character->clear_actions();

		// 更新人物速度、位置 //////////////////////////////////////////////////////////////////////////////////////////

		// 根据移动速度、环境情况计算阻力，减小速度
		auto attenuation = max(0.0f, 1 - 0.3f * time_elapsed);
		character->vel.x *= attenuation;
		character->vel.y *= attenuation;
		character->vel.z *= attenuation;

		// 计算位置移动
		character->pos.x += character->vel.x * time_elapsed;
		character->pos.y += character->vel.y * time_elapsed;
		character->pos.z += character->vel.z * time_elapsed;

		// 根据边界修正位置
		if (character->pos.x < 0) character->pos.x += _config.screen_width;
		else if (character->pos.x > _config.screen_width) character->pos.x -= _config.screen_width;

		if (character->pos.y < 0) character->pos.y += _config.screen_height;
		else if (character->pos.y > _config.screen_height) character->pos.y -= _config.screen_height;
	}
}


void Game::Impl::_render()
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
	if (_state.self->appearance >= 0)
		render_texture(_renderer, _character, &_char_clips[_state.self->appearance],
			int(_state.self->pos.x), int(_state.self->pos.y));

	//
	_gcn_gui->logic();
	_gcn_gui->draw();
	/*** This line is very slow, the frame rate get down from 2300 to 36 ***/
	auto _gcn_tex = make_sdl_ptr(SDL_CreateTextureFromSurface(_renderer.get(), _gui_surface.get()));
	render_texture(_renderer, _gcn_tex, 0, 0, TEXTURE_ORIGIN::TOP_LEFT);

	SDL_RenderPresent(_renderer.get());
}
