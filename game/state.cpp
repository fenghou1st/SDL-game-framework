#include "state.h"

#include <fstream>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "game/sdl/sdl_log.h"

using namespace std;
using namespace boost;
using namespace fenghou;
using namespace fenghou::game;


// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

State::State()
	: quit(false), frame_count(0)
{}


State::State(const string & save_file)
	: quit(false), frame_count(0)
{
	set(save_file);
}


State::~State()
{
	save();
}


void State::set(const string & save_file)
{
	_save_file = save_file;
}


bool State::load()
{
	ifstream ifs(_save_file);
	if (ifs.is_open())
	{
		archive::binary_iarchive ia(ifs);
		ia >> *this;
		post_load();
		return true;
	}

	return false;
}


bool State::save()
{
	ofstream ofs(_save_file);
	if (ofs.is_open())
	{
		archive::binary_oarchive oa(ofs);
		oa << *this;
		return true;
	}

	return false;
}


void State::post_load()
{
	self = &chars[0];
}
