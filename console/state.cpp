#include "state.h"

#include <fstream>

#include <boost/filesystem.hpp>

#include <yaml-cpp/yaml.h>

#include "game/data.h"
#include "game/sdl/sdl_log.h"

using namespace std;
using namespace fenghou;
using namespace fenghou::game;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class State::Impl
	{
	public:
		Impl(State * state);
		Impl(State * state, const string & bin_path, const string & txt_path);
		~Impl();

		void set(const string & bin_path, const string & txt_path);
		bool load();
		bool save();

	private:
		State * _state;
		Data _data;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Constants ///////////////////////////////////////////////////////////////////////////////////////////////////////

	const char SAVE_FILE_TAG[8] = { 'S', 'A', 'V', '.' , '0' , '0' , '0' , '1' };

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////

	bool load_bin(istream & src, void * dest)
	{
		if (!dest) return false;
		State & state = *reinterpret_cast<State *>(dest);

		state.quit = false;
		state.frame_count = 0;

		char file_tag[sizeof(SAVE_FILE_TAG)];
		read_bin(src, file_tag);
		if (memcmp(SAVE_FILE_TAG, file_tag, 8) != 0)
		{
			sdl::log::error("Incompatible save file");
			return false;
		}

		read_bin(src, state.curr_color);
		read_bin(src, state.curr_x);
		read_bin(src, state.curr_y);
		read_bin(src, state.screen_tips);
		return true;
	}


	bool load_txt(YAML::Node & src, void * dest)
	{
		if (!dest) return false;
		State & state = *reinterpret_cast<State *>(dest);

		state.curr_color = src["curr_color"].as<int>();
		state.curr_x = src["curr_x"].as<int>();
		state.curr_y = src["curr_y"].as<int>();
		state.screen_tips = src["screen_tips"].as<string>();
		return true;
	}


	bool save_bin(void * src, ostream & dest)
	{
		if (!src) return false;
		State & state = *reinterpret_cast<State *>(src);

		write_bin(dest, SAVE_FILE_TAG);
		write_bin(dest, state.curr_color);
		write_bin(dest, state.curr_x);
		write_bin(dest, state.curr_y);
		write_bin(dest, state.screen_tips);
		return true;
	}

	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

//// State ////////////////////////////////////////////////////////////////////////////////////////////////////////////

State::State() : impl(new Impl(this)) {}


State::State(const string & bin_path, const string & txt_path) : impl(new Impl(this, bin_path, txt_path)) {}


State::~State() { save(); }


void State::set(const string & bin_path, const string & txt_path) { return impl->set(bin_path, txt_path); }


bool State::load() { return impl->load(); }


bool State::save() { return impl->save(); }


//// State::Impl //////////////////////////////////////////////////////////////////////////////////////////////////////

State::Impl::Impl(State * state)
	: _state(state)
{}


State::Impl::Impl(State * state, const string & bin_path, const string & txt_path)
	: _state(state), _data(bin_path, txt_path, state, load_bin, load_txt, save_bin)
{}


State::Impl::~Impl()
{}


void State::Impl::set(const string & bin_path, const string & txt_path)
{
	_data.set(bin_path, txt_path, _state, load_bin, load_txt, save_bin);
}


bool State::Impl::load()
{
	return _data.load();
}


bool State::Impl::save()
{
	return _data.save();
}
