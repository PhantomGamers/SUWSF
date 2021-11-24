#pragma once
#include <string>
#include <xiosbase>
#include "dllmain.h"
#include "boost/dll.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

inline const std::string PROGRAM_PATH = boost::dll::program_location().parent_path().string();
inline const std::string LOG_PATH = PROGRAM_PATH + '/' + APPNAME + ".log";

#define DBOUT( s )	\
{	\
   std::ostringstream os_;	\
   os_ << '[' << APPNAME << "] " << s << '\n';	\
   auto str = os_.str();	\
   Logger(str);	\
   OutputDebugString( str.c_str() );  \
}

inline void Logger(std::string logMsg)
{
	std::string now = to_simple_string(boost::posix_time::second_clock::local_time());
	std::ofstream ofs(LOG_PATH.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << now << '\t' << logMsg;
	ofs.close();
}

inline void ClearLogFiles()
{
	remove(LOG_PATH.c_str());
}
