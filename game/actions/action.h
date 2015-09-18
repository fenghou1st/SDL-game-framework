//----------------------------------------------------------------------------------------------------------------------
// Action
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include <boost/serialization/serialization.hpp>

#include "game/common.h"


namespace fenghou
{
	using namespace std;
	using namespace fenghou::game;

	class Character;
	class State;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Action
	{
	public:
		virtual ~Action() {}

		virtual bool apply(Character * character, State * state) = 0;

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version) {}
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
