#pragma once

#include <cstdio>
#include <cassert>
#include <memory>

#include "game/common.h"

// User Interface //////////////////////////////////////////////////////////////////////////////////////////////////////

namespace sdl
{
	using namespace std;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Game
	{
	public:
		Game();
		~Game();

		bool is_ok() const;
		bool run();

	private:
		class Impl; unique_ptr<Impl> impl;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
