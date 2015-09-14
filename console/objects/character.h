//----------------------------------------------------------------------------------------------------------------------
// Character
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <string>
#include <vector>

#include "game/common.h"

#include "animated_object.h"
#include "../actions/action.h"

namespace fenghou
{
	using namespace std;
	using namespace fenghou::game;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Character : public AnimatedObject
	{
	public:
		virtual ~Character() {}

		bool add_action(const Action & action);


		string name;

		int_fast32_t exp;
		int_fast8_t lvl;
		double str;
		double dex;
		double wiz;
		double stamina;
		double mana;

		vector<Action> actions;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
