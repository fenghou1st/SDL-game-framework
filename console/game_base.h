#pragma once

#include <cstdio>
#include <cassert>

#include "game/common.h"


namespace sdl
{
	class GameBase
	{
	public:
		GameBase();
		~GameBase();

		bool is_ok() const { return _ok; };

	private:
		bool _ok;
	};
}
