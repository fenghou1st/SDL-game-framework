//----------------------------------------------------------------------------------------------------------------------
// Object
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include <boost/serialization/serialization.hpp>

#include "game/common.h"


namespace fenghou
{
	using namespace std;
	using namespace fenghou::game;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Object
	{
	public:
		virtual ~Object() {}

		int_fast32_t appearance;
		double hp;
		Point pos;
		Point vel;

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & appearance;
			ar & hp;
			ar & pos;
			ar & vel;
		}
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
