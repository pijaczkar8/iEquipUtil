#pragma once

#include <string>  // to_string
#include <exception>  // exception
#include <stdexcept>  // runtime_error


class bad_record_version : public std::runtime_error
{
public:
	explicit bad_record_version(UInt32 a_versionExpected, UInt32 a_versionFound) :
		runtime_error("Record is out of date! Expected (" + std::to_string(a_versionExpected) + "), found (" + std::to_string(a_versionFound) + ")!")
	{}
};
