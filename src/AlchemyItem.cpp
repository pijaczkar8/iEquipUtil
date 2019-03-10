#include "AlchemyItem.h"

#include "version.h"  // MAKE_STR


namespace Forms
{
	AlchemyItem::AlchemyItem(bool a_useHandle) :
		ISerializableForm(a_useHandle)
	{}


	AlchemyItem::~AlchemyItem()
	{}


	const char* AlchemyItem::ClassName() const
	{
		return MAKE_STR(AlchemyItem);
	}


	FormType AlchemyItem::GetType() const
	{
		return FormType::kFormType_Potion;
	}


	void AlchemyItem::Set(TESForm* a_form, BaseExtraList* a_extraList)
	{
		if (_refHandle.IsActive()) {
			_refHandle.AcquireHandle();
		}
	}


	int AlchemyItem::Comp(const ISerializableForm* a_rhs) const
	{
		int result = GetLoadedFormID() - a_rhs->GetLoadedFormID();
		if (result != 0) {
			return result < 0 ? -1 : 1;
		} else {
			return 0;
		}
	}
}
