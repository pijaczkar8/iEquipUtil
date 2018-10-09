#pragma once

#include "ITypes.h"  // SInt32, UInt64

#include <string>  // string


namespace iEquip_Utility
{
	extern bool GIST;
	extern SInt32 GISTIndex;


	bool checkForGIST();
	std::string boolToString(bool a_bool);
	std::string numToHexString(UInt64 a_num, UInt64 a_bytes = 4);
}