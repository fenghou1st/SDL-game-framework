//----------------------------------------------------------------------------------------------------------------------
// Point
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <boost/serialization/serialization.hpp>


namespace fenghou { namespace game
{
	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Point
	{
	public:

		double x;
		double y;
		double z;

	private:
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int version)
		{
			ar & x;
			ar & y;
			ar & z;
		}
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}}
