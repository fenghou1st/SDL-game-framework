//----------------------------------------------------------------------------------------------------------------------
// Action: move
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include "game/common.h"
#include "action.h"

namespace fenghou
{
	using namespace std;
	using namespace fenghou::game;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Move : public Action
	{
	public:
		Move(double phi, double theta, double power_ratio) : phi(phi), theta(theta), power_ratio(power_ratio) {}
		virtual ~Move() {}

		virtual bool apply(Character * character, State * state);

		double phi; // radian
		double theta; // radian
		double power_ratio; // [0, 1]
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
