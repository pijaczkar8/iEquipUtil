#include "ExtraPoison.h"

#include <exception>  // exception

#include "version.h"  // MAKE_STR
#include "RE/ExtraPoison.h"  // RE::ExtraPoison


namespace iEquip
{
	ExtraPoison::ExtraPoison() :
		IExtraDataType(),
		_poison(false),
		_count(0)
	{}


	ExtraPoison::~ExtraPoison()
	{}


	const char* ExtraPoison::ClassName() const
	{
		return MAKE_STR(ExtraPoison);
	}


	void ExtraPoison::Clear()
	{
		_poison.Clear();
		_count = 0;
	}


	bool ExtraPoison::Save(json& a_save)
	{
		try {
			json poisonSave;
			if (!_poison.Save(poisonSave)) {
				return false;
			}

			a_save = {
				{ _poison.ClassName(), poisonSave },
				{ MAKE_STR(_count), _count }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool ExtraPoison::Load(json& a_load)
	{
		try {
			auto& it = a_load.find(_poison.ClassName());
			if (it == a_load.end() || !_poison.Load(*it)) {
				return false;
			}

			if (!loadJsonObj(a_load, MAKE_STR(_count), _count)) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	ExtraDataType ExtraPoison::GetType() const
	{
		return ExtraDataType::kExtraData_Poison;
	}


	void ExtraPoison::Set(const BSExtraData* a_extraData)
	{
		const RE::ExtraPoison* xPoison = static_cast<const RE::ExtraPoison*>(a_extraData);
		if (xPoison->poison) {
			_poison.SetForm(xPoison->poison->formID);
		}
		_count = xPoison->count;
	}


	int ExtraPoison::Comp(const IExtraDataType* a_rhs) const
	{
		const ExtraPoison* rhs = static_cast<const ExtraPoison*>(a_rhs);
		int result = _poison.Compare(rhs->_poison);
		if (result != 0) {
			return result < 0 ? -1 : 1;
		} else {
			result = _count - rhs->_count;
			if (result != 0) {
				return result < 0 ? -1 : 1;
			} else {
				return 0;
			}
		}
	}
}
