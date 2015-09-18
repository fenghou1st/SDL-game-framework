//----------------------------------------------------------------------------------------------------------------------
// Game data I/O
//
// bin file: for speed
// txt file: for platform compatibility
//
// FH_GAME_DATA_LOAD_TXT_FIRST:
//   The default behavior is: try to load bin file first, if failed, then try to load txt file.
//   If add this flag while compiling this module, it will try to load txt file first.
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <memory>
#include <fstream>

#include <yaml-cpp/yaml.h>

// User Interface //////////////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou { namespace game
{
	using namespace std;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Data
	{
	public:
		typedef bool(*callback_load_bin)(istream & src, void * dest);
		typedef bool(*callback_load_txt)(YAML::Node & src, void * dest);
		typedef bool(*callback_save_bin)(void * src, ostream & dest);

		Data();
		Data(const string & bin_path, const string & txt_path, void * data_obj,
			callback_load_bin load_bin, callback_load_txt load_txt, callback_save_bin save_bin = nullptr);
		Data(const string & bin_path, void * data_obj,
			callback_load_bin load_bin, callback_save_bin save_bin = nullptr);
		~Data();

		void set(const string & bin_path, const string & txt_path, void * data_obj,
			callback_load_bin load_bin, callback_load_txt load_txt, callback_save_bin save_bin = nullptr);
		void set(const string & bin_path, void * data_obj,
			callback_load_bin load_bin, callback_save_bin save_bin = nullptr);
		bool load();
		bool save();

	private:
		class Impl; unique_ptr<Impl> impl;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T> inline bool read_bin(istream & s, T & value)
	{
		s.read(reinterpret_cast<char *>(&value), sizeof(T));
		return s.good();
	}


	template<> inline bool read_bin(istream & s, string & value)
	{
		uint16_t len;
		s.read(reinterpret_cast<char *>(&len), sizeof(uint16_t));
		if (!s.good()) return false;

		value.resize(len);
		if (len > 0) s.read(&value.front(), len);
		return s.good();
	}


	template<typename T> inline bool write_bin(ostream & s, const T & value)
	{
		s.write(reinterpret_cast<const char *>(&value), sizeof(T));
		return s.good();
	}


	template<> inline bool write_bin(ostream & s, const string & value)
	{
		uint16_t len = value.size();
		s.write(reinterpret_cast<const char *>(&len), sizeof(uint16_t));
		if (!s.good()) return false;

		if (len > 0) s.write(value.data(), len);
		return s.good();
	}

	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}}
