#include "Armor.h"

#include "GameBSExtraData.h"  // ExtraDataType
#include "GameExtraData.h"  // InventoryEntryData

#include <exception>  // exception

#include "version.h"  // MAKE_STR


namespace iEquip
{
	Armor::Armor() :
		ISerializableForm()
	{}


	Armor::~Armor()
	{}


	const char* Armor::ClassName() const
	{
		return MAKE_STR(Armor);
	}


	void Armor::Clear()
	{
		_extraData.Clear();
	}


	bool Armor::Save(json& a_save)
	{
		try {
			json ArmorSave;
			if (!ISerializableForm::Save(ArmorSave)) {
				return false;
			}

			json xDataSave;
			if (!_extraData.Save(xDataSave)) {
				return false;
			}

			a_save = {
				{ ClassName(), ArmorSave },
				{ _extraData.ClassName(), xDataSave }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s\n", e.what());
			return false;
		}

		return true;
	}


	bool Armor::Load(json& a_load)
	{
		try {
			auto& it = a_load.find(ClassName());
			if (it == a_load.end() || !ISerializableForm::Load(*it)) {
				return false;
			}

			it = a_load.find(_extraData.ClassName());
			if (it == a_load.end() || !_extraData.Load(*it)) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s\n", e.what());
			return false;
		}

		return true;
	}


	UInt32 Armor::GetType() const
	{
		return FormType::kFormType_Armor;
	}


	void Armor::Set(TESForm* a_form, BaseExtraList* a_extraList)
	{
		SetForm(a_form->formID);
		if (a_extraList) {
			_extraData.Parse(a_extraList);
		}
	}


	int Armor::Comp(const ISerializableForm* a_rhs) const
	{
		const Armor* rhs = static_cast<const Armor*>(a_rhs);
		int result = GetLoadedFormID() - rhs->GetLoadedFormID();
		if (result != 0) {
			return result < 0 ? -1 : 1;
		} else {
			return _extraData.Compare(rhs->_extraData);
		}
	}
}
