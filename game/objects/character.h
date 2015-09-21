//----------------------------------------------------------------------------------------------------------------------
// Character
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <string>
#include <vector>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "game/common.h"

#include "animated_object.h"
#include "../actions/actions.h"

namespace fenghou
{
	using namespace std;
	using namespace fenghou::game;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Character : public AnimatedObject
	{
	public:
		Character() {}
		Character(const Character &) = delete;
		Character & operator=(const Character &) = delete;
		virtual ~Character();

		bool add_action(Action * action);
		void clear_actions();

		string name;

		int_fast32_t exp;
		int_fast16_t lvl;
		double str;
		double dex;
		double wiz;
		double stamina;
		double mana;

		vector<Action *> actions;

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar.register_type(static_cast<Move *>(nullptr));

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
