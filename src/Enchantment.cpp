#include "Enchantment.h"

#include "GameForms.h"  // FormType

#include "version.h"  // MAKE_STR


namespace iEquip
{
	Enchantment::Enchantment() :
		ISerializableForm(false)
	{}


	Enchantment::~Enchantment()
	{}


	const char* Enchantment::ClassName() const
	{
		return MAKE_STR(Enchantment);
	}


	FormType Enchantment::GetType() const
	{
		return FormType::kFormType_Enchantment;
	}


	void Enchantment::Set(TESForm* a_form, BaseExtraList* a_extraList)
	{}


	int Enchantment::Comp(const ISerializableForm* a_rhs) const
	{
		const Enchantment* rhs = static_cast<const Enchantment*>(a_rhs);
		int result = GetLoadedFormID() - rhs->GetLoadedFormID();
		if (result != 0) {
			return result < 0 ? -1 : 1;
		} else {
			return 0;
		}
	}
}
