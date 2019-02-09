#pragma once

#include "ISerializableForm.h"  // ISerializableForm


namespace iEquip
{
	class AlchemyItem : public ISerializableForm
	{
	public:
		AlchemyItem() = delete;
		explicit AlchemyItem(bool a_useHandle);
		AlchemyItem(const AlchemyItem&) = default;
		AlchemyItem(AlchemyItem&&) = default;
		virtual ~AlchemyItem();

		virtual const char*	ClassName() const override;
		virtual FormType	GetType() const override;
		virtual void		Set(TESForm* a_form, BaseExtraList* a_extraList) override;

	protected:
		virtual int			Comp(const ISerializableForm* a_rhs) const override;
	};
}
