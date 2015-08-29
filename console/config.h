//----------------------------------------------------------------------------------------------------------------------
// Configuration I/O
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <memory>
#include <string>

// User Interface //////////////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou
{
	using namespace std;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Config
	{
	public:
		Config();
		Config(const string & file_path);
		~Config();

		void set(const string & file_path);
		bool load();
		bool save();

		// Begin: config item //////////////////////////////////////////////////////////////////////////////////////////
		int screen_width;
		int screen_height;
		int frame_rate;
		// End: config item ////////////////////////////////////////////////////////////////////////////////////////////

	private:
		class Impl; unique_ptr<Impl> impl;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}
