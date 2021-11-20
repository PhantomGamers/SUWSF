#pragma once
#include <string>
#include <xiosbase>
#include "dllmain.h"

#define DBOUT( s )	\
{	\
   std::ostringstream os_;	\
   os_ << '[' << APPNAME << "] " << s << '\n';	\
   auto str = os_.str();	\
   Logger(str);	\
   OutputDebugString( str.c_str() );  \
}

// modified from https://stackoverflow.com/a/46866854
inline std::string getCurrentDateTime(std::string s) {
	time_t now = time(0);
	struct tm tstruct;
	char buf[26];
	localtime_s(&tstruct, &now);
	if (s == "now")
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (s == "date")
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return std::string(buf);
};

inline void Logger(std::string logMsg) {
	std::string filePath = APPNAME + getCurrentDateTime("date") + ".log";
	std::string now = getCurrentDateTime("now");
	std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << now << '\t' << logMsg << '\n';
	ofs.close();
}