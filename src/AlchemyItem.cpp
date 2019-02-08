#include "AlchemyItem.h"

#include "version.h"  // MAKE_STR


namespace iEquip
{
	AlchemyItem::AlchemyItem() :
		ISerializableForm()
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
	{}


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
