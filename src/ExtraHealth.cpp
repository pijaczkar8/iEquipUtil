#include "ExtraHealth.h"

#include "GameBSExtraData.h"  // ExtraDataType
#include "GameExtraData.h"  // ExtraHealth

#include <exception>  // exception

#include "version.h"  // MAKE_STR


namespace iEquip
{
	ExtraHealth::ExtraHealth() :
		_health(0.0)
	{}


	ExtraHealth::~ExtraHealth()
	{}


	const char* ExtraHealth::ClassName() const
	{
		return MAKE_STR(ExtraHealth);
	}


	void ExtraHealth::Clear()
	{
		_health = 0.0;
	}


	bool ExtraHealth::Save(json& a_save)
	{
		try {
			a_save = {
				{ MAKE_STR(_health), _health }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool ExtraHealth::Load(json& a_load)
	{
		try {
			if (!loadJsonObj(a_load, MAKE_STR(_health), _health)) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	ExtraDataType ExtraHealth::GetType() const
	{
		return ExtraDataType::kExtraData_Health;
	}


	void ExtraHealth::Set(const BSExtraData* a_extraData)
	{
		_health = static_cast<const ::ExtraHealth*>(a_extraData)->health;
	}


	int ExtraHealth::Comp(const IExtraDataType* a_rhs) const
	{
		const ExtraHealth* rhs = static_cast<const ExtraHealth*>(a_rhs);
		float result = _health - rhs->_health;
		if (result != 0.0) {
			return result < 0.0 ? -1 : 1;
		} else {
			return 0;
		}
	}
}
