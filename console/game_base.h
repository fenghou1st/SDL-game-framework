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

	const string DIR_BASE_BIN = "bin";
	const string DIR_BASE_DATA = "data";
	const string DIR_PREF_CFG = "configs";
	const string DIR_PREF_LOG = "logs";
	const string DIR_PREF_SAVE = "saves";
}
