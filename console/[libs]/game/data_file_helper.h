//----------------------------------------------------------------------------------------------------------------------
// Explanation
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <fstream>

// User Interface //////////////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou
{
	namespace game
	{
		namespace data_file_helper
		{
			using namespace std;

			// Classes /////////////////////////////////////////////////////////////////////////////////////////////////
			// Functions ///////////////////////////////////////////////////////////////////////////////////////////////

			template<typename T> inline bool read(ifstream & stream, T & value)
			{
				stream.read(reinterpret_cast<char *>(&value), sizeof(T));
				return stream.good();
			}


			template<> inline bool read(ifstream & stream, string & value)
			{
				uint16_t len;
				stream.read(reinterpret_cast<char *>(&len), sizeof(uint16_t));
				if (!stream.good()) return false;

				value.resize(len);
				stream.read(&value.front(), len);
				return stream.good();
			}


			template<typename T> inline bool write(ofstream & stream, const T & value)
			{
				stream.write(reinterpret_cast<const char *>(&value), sizeof(T));
				return stream.good();
			}


			template<> inline bool write(ofstream & stream, const string & value)
			{
				uint16_t len = value.size();
				stream.write(reinterpret_cast<const char *>(&len), sizeof(uint16_t));
				if (!stream.good()) return false;

				stream.write(value.data(), len);
				return stream.good();
			}

			// Global Variables ////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////
