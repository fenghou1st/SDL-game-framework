//----------------------------------------------------------------------------------------------------------------------
// Game state I/O
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include "objects/character.h"


namespace fenghou
{
	using namespace std;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class State
	{
	public:
		State();
		State(const string & save_file);
		~State();

		void set(const string & save_file);
		bool load();
		bool save();

		bool quit;
		uint_fast64_t frame_count;
		Character * self;

		vector<Character> chars;
		string screen_tips;

	private:
		void post_load();

		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & chars;
			ar & screen_tips;
		}

		string _save_file;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
