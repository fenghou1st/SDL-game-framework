//----------------------------------------------------------------------------------------------------------------------
// Action: move
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include <boost/serialization/serialization.hpp>

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
		Move() {}
		Move(double phi, double theta, double power_ratio) : phi(phi), theta(theta), power_ratio(power_ratio) {}
		virtual ~Move() {}

		virtual bool apply(Character * character, State * state);

		double phi; // radian
		double theta; // radian
		double power_ratio; // [0, 1]

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Action>(*this);
			ar & phi;
			ar & theta;
			ar & power_ratio;
		}
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
