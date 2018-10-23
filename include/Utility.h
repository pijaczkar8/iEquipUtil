#pragma once

#include "ITypes.h"  // UInt32, UInt64
#include "GameForms.h"  // BGSKeyword

#include <string>  // string


namespace iEquip
{
	enum Keyword : UInt32
	{
		kKeyword_Javelin = 0x0073FB,
		kKeyword_WAF_WeapTypeGrenade = 0xAF0237
	};


	std::string boolToString(bool a_bool);
	std::string numToHexString(UInt64 a_num, UInt64 a_bytes = 4);
	BGSKeyword* GetKeyword(Keyword a_keyword, std::string a_modName);


	static const std::string NAME_SpearsBySoolie = "SpearsBySoolie.esp";
	static const std::string NAME_Update = "Update.esm";
}