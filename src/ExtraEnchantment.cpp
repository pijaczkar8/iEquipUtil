#include "ExtraEnchantment.h"

#include "GameBSExtraData.h"  // ExtraDataType
#include "GameExtraData.h"  // ExtraEnchantment

#include <exception>  // exception

#include "version.h"  // MAKE_STR


namespace iEquip
{
	ExtraEnchantment::ExtraEnchantment() :
		IExtraDataType()
	{}


	ExtraEnchantment::~ExtraEnchantment()
	{}


	const char* ExtraEnchantment::ClassName() const
	{
		return MAKE_STR(ExtraEnchantment);
	}


	void ExtraEnchantment::Clear()
	{
		_enchantment.Clear();
	}


	bool ExtraEnchantment::Save(json& a_save)
	{
		try {
			json enchSave;
			if (!_enchantment.Save(enchSave)) {
				return false;
			}

			a_save = {
				{ _enchantment.ClassName(), enchSave }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool ExtraEnchantment::Load(json& a_load)
	{
		try {
			auto& it = a_load.find(_enchantment.ClassName());
			if (it == a_load.end() || !_enchantment.Load(*it)) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	ExtraDataType ExtraEnchantment::GetType() const
	{
		return ExtraDataType::kExtraData_Enchantment;
	}


	void ExtraEnchantment::Set(const BSExtraData* a_extraData)
	{
		const ::ExtraEnchantment* xEnch = static_cast<const ::ExtraEnchantment*>(a_extraData);
		if (xEnch->enchant) {
			_enchantment.SetForm(xEnch->enchant->formID);
		}
	}


	int ExtraEnchantment::Comp(const IExtraDataType* a_rhs) const
	{
		return _enchantment.Compare(static_cast<const ExtraEnchantment*>(a_rhs)->_enchantment);
	}
}
