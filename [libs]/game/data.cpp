#include "data.h"

#include <fstream>

#include <boost/filesystem.hpp>

using namespace std;
using namespace fenghou;
using namespace fenghou::game;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou { namespace game
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Data::Impl
	{
	public:
		Impl();
		~Impl();

		void set(const string & bin_path, const string & txt_path, void * data_obj,
			callback_load_bin load_bin, callback_load_txt load_txt, callback_save_bin save_bin);
		bool load();
		bool save();

	private:
		bool load_bin();
		bool load_txt();
		bool save_bin();

		string _bin_path;
		string _txt_path;
		void * _data_obj;
		callback_load_bin _load_bin;
		callback_load_txt _load_txt;
		callback_save_bin _save_bin;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}}

// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

//// Data /////////////////////////////////////////////////////////////////////////////////////////////////////////////

Data::Data() : impl(new Impl) {}


Data::Data(const string & bin_path, const string & txt_path, void * data_obj,
	callback_load_bin load_bin, callback_load_txt load_txt, callback_save_bin save_bin)
	: impl(new Impl)
{
	impl->set(bin_path, txt_path, data_obj, load_bin, load_txt, save_bin);
}


Data::Data(const string & bin_path, void * data_obj, callback_load_bin load_bin, callback_save_bin save_bin)
	: impl(new Impl)
{
	impl->set(bin_path, nullptr, data_obj, load_bin, nullptr, save_bin);
}


Data::~Data() { save(); }


void Data::set(const string & bin_path, const string & txt_path, void * data_obj,
	callback_load_bin load_bin, callback_load_txt load_txt, callback_save_bin save_bin)
{
	impl->set(bin_path, txt_path, data_obj, load_bin, load_txt, save_bin);
}


void Data::set(const string & bin_path, void * data_obj, callback_load_bin load_bin, callback_save_bin save_bin)
{
	impl->set(bin_path, nullptr, data_obj, load_bin, nullptr, save_bin);
}


bool Data::load() { return impl->load(); }


bool Data::save() { return impl->save(); }


//// Data::Impl ///////////////////////////////////////////////////////////////////////////////////////////////////////

Data::Impl::Impl()
{}


Data::Impl::~Impl()
{}


void Data::Impl::set(const string & bin_path, const string & txt_path, void * data_obj,
	callback_load_bin load_bin, callback_load_txt load_txt, callback_save_bin save_bin)
{
	_bin_path = bin_path;
	_txt_path = txt_path;
	_data_obj = data_obj;
	_load_bin = load_bin;
	_load_txt = load_txt;
	_save_bin = save_bin;
}


// TAG: txt_first
bool Data::Impl::load()
{
	bool loaded = false;

#ifndef FH_GAME_DATA_LOAD_TXT_FIRST
	if (!loaded && !_bin_path.empty() && _load_bin && _data_obj) loaded = load_bin();
	if (!loaded && !_txt_path.empty() && _load_txt && _data_obj) loaded = load_txt();
#else
	if (!loaded && !_txt_path.empty() && _load_txt && _data_obj) loaded = load_txt();
	if (!loaded && !_bin_path.empty() && _load_bin && _data_obj) loaded = load_bin();
#endif // FH_GAME_DATA_LOAD_TXT_FIRST

	return loaded;
}


bool Data::Impl::save()
{
	bool saved = false;

	if (!saved && !_bin_path.empty() && _save_bin && _data_obj) saved = save_bin();

	return saved;
}


bool Data::Impl::load_bin()
{
	if (!boost::filesystem::exists(_bin_path) || boost::filesystem::file_size(_bin_path) <= 0) return false;

	ifstream s(_bin_path, ios::binary);

	return _load_bin(s, _data_obj);
}


bool Data::Impl::load_txt()
{
	if (!boost::filesystem::exists(_txt_path) || boost::filesystem::file_size(_txt_path) <= 0) return false;

	auto node = YAML::LoadFile(_txt_path);

	return _load_txt(node, _data_obj);
}


bool Data::Impl::save_bin()
{
	ofstream s(_bin_path, ios::binary | ios::trunc);

	return _save_bin(_data_obj, s);
}
