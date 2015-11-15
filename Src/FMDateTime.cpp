#include "FMDateTime.h"

const std::string FMGetCurrentDateTime()
{
	time_t		now = time(0);
	char		buf[80];
	struct tm tstruct;

	tstruct = *localtime(&now);
	strftime (buf, sizeof(buf), "%Y-%m-%d %H_%M_%S", &tstruct);
	

	return buf;

}