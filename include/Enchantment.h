#pragma once

#include "GameForms.h"  // EffectSetting

#include "ISerializableForm.h"  // ISerializableForm


namespace Forms
{
	class Enchantment : public ISerializableForm
	{
	public:
		Enchantment() = delete;
		explicit Enchantment(bool a_useHandle);
		Enchantment(const Enchantment&) = default;
		Enchantment(Enchantment&&) = default;
		virtual ~Enchantment();

		virtual const char*	ClassName() const override;
		virtual FormType	GetType() const override;
		virtual void		Set(TESForm* a_form, BaseExtraList* a_extraList) override;

	protected:
		virtual int			Comp(const ISerializableForm* a_rhs) const override;
	};
}
