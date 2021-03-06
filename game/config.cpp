#include "config.h"

#ifndef _WIN32
#include <unistd.h>
#endif
#include <fstream>

#include <boost/filesystem.hpp>

#define SI_SUPPORT_IOSTREAMS
#include "simple_ini/SimpleIni.h"

#include "game_base.h"

using namespace std;
using namespace fenghou;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Config::Impl
	{
	public:
		Impl(Config & config);
		~Impl();

		void set(const string & file_path);
		bool load();
		bool save();

	private:
		Config & _config;
		string _file_path;
		CSimpleIniA _ini;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

//// Config ////////////////////////////////////////////////////////////////////////////////////////////////////////////

Config::Config() : impl(new Impl(*this)) {}


Config::Config(const string & file_path) : impl(new Impl(*this)) { impl->set(file_path); }


Config::~Config() { save(); }


void Config::set(const string & file_path) { return impl->set(file_path); }


bool Config::load() { return impl->load(); }


bool Config::save() { return impl->save(); }


//// Config::Impl //////////////////////////////////////////////////////////////////////////////////////////////////////

Config::Impl::Impl(Config & config)
	: _config(config), _ini(true)
{}


Config::Impl::~Impl()
{}


void Config::Impl::set(const string & file_path)
{
	_file_path = file_path;
}


bool Config::Impl::load()
{
	// create the file if not exist (but not create the missing directories)
	if (!boost::filesystem::exists(_file_path)) _ini.SaveFile(_file_path.c_str());

	SI_Error rc = _ini.LoadFile(_file_path.c_str());
	if (rc < 0) return false;

	// Begin: get data /////////////////////////////////////////////////////////////////////////////////////////////////
	_config.screen_width = _ini.GetLongValue("System", "Screen Width", 1600);
	_config.screen_height = _ini.GetLongValue("System", "Screen Height", 900);
	_config.frame_rate = _ini.GetLongValue("System", "Frame Rate", 60);
	// End: get data ///////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}


bool Config::Impl::save()
{
	// Begin: set data /////////////////////////////////////////////////////////////////////////////////////////////////
	_ini.SetLongValue("System", "Screen Width", _config.screen_width);
	_ini.SetLongValue("System", "Screen Height", _config.screen_height);
	_ini.SetLongValue("System", "Frame Rate", _config.frame_rate);
	// End: set data ///////////////////////////////////////////////////////////////////////////////////////////////////

	auto rc = _ini.SaveFile(_file_path.c_str());
	return rc >= 0;
}
