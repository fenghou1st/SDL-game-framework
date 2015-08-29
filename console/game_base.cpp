#include "game_base.h"

#include <boost/filesystem.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "game/sdl/sdl_log.h"

using namespace std;
using namespace sdl;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////

namespace sdl
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class GameBase::Impl
	{
	public:
		Impl(const string & org, const string & app);
		~Impl();

		bool is_ok() const { return _ok; }
		static string get_data_path() { return _data_path; }
		static string get_pref_path() { return _pref_path; }

	private:
		static bool _init_data_path();
		static bool _init_pref_path(const string & org, const string & app);

		bool _ok;
		static string _data_path;
		static string _pref_path;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////

	string GameBase::Impl::_data_path;
	string GameBase::Impl::_pref_path;
}


// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

//// GameBase //////////////////////////////////////////////////////////////////////////////////////////////////////////

GameBase::GameBase(const string & org, const string & app) : impl(new Impl(org, app)) {}


GameBase::~GameBase() {}


bool GameBase::is_ok() const { return impl->is_ok(); }


string GameBase::get_data_path() { return Impl::get_data_path(); }


string GameBase::get_pref_path() { return Impl::get_pref_path(); }


//// GameBase::Impl ////////////////////////////////////////////////////////////////////////////////////////////////////

GameBase::Impl::Impl(const string & org, const string & app)
	: _ok(false)
{
	if (!_init_pref_path(org, app)) return;

	log::init(_pref_path + DIR_PREF_LOG + PATH_SEP);

	if (!_init_data_path())
	{
		log::error("_init_data_path");
		return;
	}

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


GameBase::Impl::~Impl()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


bool GameBase::Impl::_init_data_path()
{
	auto base_path = SDL_GetBasePath();
	if (!base_path)
	{
		log::error("SDL_GetBasePath");
		return false;
	}

	_data_path = base_path;
	SDL_free(base_path);

	auto pos = _data_path.rfind(DIR_BASE_BIN);
	if (pos == string::npos || pos + DIR_BASE_BIN.size() + 1 != _data_path.size())
	{
		log::error("Executable file not in bin directory");
		return false;
	}

	_data_path = _data_path.substr(0, pos) + DIR_BASE_DATA + PATH_SEP;
	return true;
}


// This function will be called before log::init, so can't write log
bool GameBase::Impl::_init_pref_path(const string & org, const string & app)
{
	auto pref_path = SDL_GetPrefPath(org.c_str(), app.c_str());
	if (pref_path == nullptr) return false;

	_pref_path = pref_path;
	SDL_free(pref_path);

	if (!boost::filesystem::exists(_pref_path)) boost::filesystem::create_directories(_pref_path);

	auto cfg_path = _pref_path + DIR_PREF_CFG + PATH_SEP;
	if (!boost::filesystem::exists(cfg_path)) boost::filesystem::create_directories(cfg_path);

	auto log_path = _pref_path + DIR_PREF_LOG + PATH_SEP;
	if (!boost::filesystem::exists(log_path)) boost::filesystem::create_directories(log_path);

	auto save_path = _pref_path + DIR_PREF_SAVE + PATH_SEP;
	if (!boost::filesystem::exists(save_path)) boost::filesystem::create_directories(save_path);

	return true;
}
