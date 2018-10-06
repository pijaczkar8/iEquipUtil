#pragma once

#include "common/ITypes.h"  // UInt32

#include <string>  // string


namespace iEquip_Utility
{
	extern bool GIST;
	extern UInt32 GISTIndex;

	bool checkForGIST();
	std::string boolToString(bool a_bool);
	std::string numToHexString(UInt64 a_num, UInt64 a_bytes);
}