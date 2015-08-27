#pragma once

#include <cstdio>
#include <cassert>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include <SDL.h>


namespace sdl
{
	namespace log
	{
		namespace logging = boost::log;
		namespace src = boost::log::sources;
		namespace sinks = boost::log::sinks;
		namespace keywords = boost::log::keywords;
		namespace expr = boost::log::expressions;


		inline void init(const string & pref_path)
		{
			logging::add_console_log(std::clog,
				keywords::filter = (logging::trivial::severity >= logging::trivial::info),
				keywords::format = (expr::stream
					<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
					<< "] <" << logging::trivial::severity << "> " << expr::smessage
					)
				);

			logging::add_file_log(
				keywords::file_name = pref_path + "game_%Y-%m-%d.log",
				keywords::open_mode = std::ios_base::app,
				keywords::rotation_size = 10 * 1024 * 1024,
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::auto_flush = true,
				keywords::format = (expr::stream
					<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
					<< "] <" << logging::trivial::severity << "> " << expr::smessage
					)
				);

			logging::add_common_attributes();
		}


		inline void trace(const std::string & msg)
		{
			src::severity_logger<logging::trivial::severity_level> lg;
			BOOST_LOG_SEV(lg, logging::trivial::trace) << msg << " [" << SDL_GetError() << "]";
		}


		inline void debug(const std::string & msg)
		{
			src::severity_logger<logging::trivial::severity_level> lg;
			BOOST_LOG_SEV(lg, logging::trivial::debug) << msg << " [" << SDL_GetError() << "]";
		}


		inline void info(const std::string & msg)
		{
			src::severity_logger<logging::trivial::severity_level> lg;
			BOOST_LOG_SEV(lg, logging::trivial::info) << msg << " [" << SDL_GetError() << "]";
		}


		inline void warning(const std::string & msg)
		{
			src::severity_logger<logging::trivial::severity_level> lg;
			BOOST_LOG_SEV(lg, logging::trivial::warning) << msg << " [" << SDL_GetError() << "]";
		}


		inline void error(const std::string & msg)
		{
			src::severity_logger<logging::trivial::severity_level> lg;
			BOOST_LOG_SEV(lg, logging::trivial::error) << msg << " [" << SDL_GetError() << "]";
		}


		inline void fatal(const std::string & msg)
		{
			src::severity_logger<logging::trivial::severity_level> lg;
			BOOST_LOG_SEV(lg, logging::trivial::fatal) << msg << " [" << SDL_GetError() << "]";
		}
	}
}
