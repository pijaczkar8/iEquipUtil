#pragma once

#include "Enchantment.h"  // Enchantment
#include "IExtraDataType.h"  // IExtraDataType


namespace Forms
{
	class ExtraEnchantment : public IExtraDataType
	{
	public:
		ExtraEnchantment();
		ExtraEnchantment(const ExtraEnchantment&) = default;
		ExtraEnchantment(ExtraEnchantment&&) = default;
		virtual ~ExtraEnchantment();

		virtual const char*		ClassName() const override;
		virtual void			Clear() override;
		virtual bool			Save(json& a_save) override;
		virtual bool			Load(json& a_load) override;
		virtual ExtraDataType	GetType() const override;
		virtual void			Set(const BSExtraData* a_extraData) override;

	protected:
		virtual int				Comp(const IExtraDataType* a_rhs) const override;


		Enchantment _enchantment;
	};
}
