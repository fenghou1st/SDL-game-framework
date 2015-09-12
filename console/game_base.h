#pragma once

#include <cstdio>
#include <cassert>
#include <memory>
#include <string>

#include "game/common.h"

// User Interface //////////////////////////////////////////////////////////////////////////////////////////////////////

namespace sdl
{
	using namespace std;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class GameBase
	{
	public:
		GameBase(const string & org, const string & app);
		~GameBase();

		bool is_ok() const;

		static string get_data_path();
		static string get_pref_path();

	private:
		class Impl; unique_ptr<Impl> impl;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	const uint32_t RMASK = 0xff000000;
	const uint32_t GMASK = 0x00ff0000;
	const uint32_t BMASK = 0x0000ff00;
	const uint32_t AMASK = 0x000000ff;
#else
	const uint32_t RMASK = 0x000000ff;
	const uint32_t GMASK = 0x0000ff00;
	const uint32_t BMASK = 0x00ff0000;
	const uint32_t AMASK = 0xff000000;
#endif

	const string DIR_BASE_BIN = "bin";
	const string DIR_BASE_DATA = "data";
	const string DIR_PREF_CFG = "configs";
	const string DIR_PREF_LOG = "logs";
	const string DIR_PREF_SAVE = "saves";
}
