//----------------------------------------------------------------------------------------------------------------------
// Animated Object
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include <boost/serialization/serialization.hpp>

#include "game/common.h"

#include "object.h"


namespace fenghou
{
	using namespace std;
	using namespace fenghou::game;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class AnimatedObject : public Object
	{
	public:
		virtual ~AnimatedObject() {}

		int_fast32_t ani;
		double ani_progress;

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Object>(*this);
			ar & ani;
			ar & ani_progress;
		}
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
