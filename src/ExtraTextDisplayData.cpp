#include "ExtraTextDisplayData.h"

#include "GameExtraData.h"  // ExtraTextDisplayData

#include <exception>  // exception

#include "version.h"  // MAKE_STR


namespace Forms
{
	ExtraTextDisplayData::ExtraTextDisplayData() :
		_name("")
	{}


	ExtraTextDisplayData::~ExtraTextDisplayData()
	{}


	const char* ExtraTextDisplayData::ClassName() const
	{
		return MAKE_STR(ExtraTextDisplayData);
	}


	void ExtraTextDisplayData::Clear()
	{
		_name.clear();
	}


	bool ExtraTextDisplayData::Save(json& a_save)
	{
		try {
			a_save = {
				{ MAKE_STR(_name), _name }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool ExtraTextDisplayData::Load(json& a_load)
	{
		try {
			if (!loadJsonObj(a_load, MAKE_STR(_name), _name)) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	ExtraDataType ExtraTextDisplayData::GetType() const
	{
		return ExtraDataType::kExtraData_TextDisplayData;
	}


	void ExtraTextDisplayData::Set(const BSExtraData* a_extraData)
	{
		_name = static_cast<const ::ExtraTextDisplayData*>(a_extraData)->name.data;
	}


	int ExtraTextDisplayData::Comp(const IExtraDataType* a_rhs) const
	{
		return _name.compare(static_cast<const ExtraTextDisplayData*>(a_rhs)->_name);
	}
}
