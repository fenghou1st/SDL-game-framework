//----------------------------------------------------------------------------------------------------------------------
// Character
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <string>
#include <vector>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>

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

		bool add_action(const std::shared_ptr<Action> & action);

		string name;

		int_fast32_t exp;
		int_fast8_t lvl;
		double str;
		double dex;
		double wiz;
		double stamina;
		double mana;

		vector<std::shared_ptr<Action>> actions;

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<AnimatedObject>(*this);
			ar & name;
			ar & exp;
			ar & lvl;
			ar & str;
			ar & dex;
			ar & wiz;
			ar & stamina;
			ar & mana;
			ar & actions;
		}
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
