#include "version.h"

std::string version()
{
	std::stringstream ss;
	ss << PROJECT_VERSION_MAJOR << ".";
	ss << PROJECT_VERSION_MINOR << ".";
	ss << PROJECT_VERSION_PATCH;
	std::string str = ss.str();
	return str;
}

