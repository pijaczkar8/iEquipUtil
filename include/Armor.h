#pragma once

#include "ExtraData.h"  // ExtraData
#include "ISerializableForm.h"  // ISerializableForm


namespace iEquip
{
	class Armor : public ISerializableForm
	{
	public:
		Armor() = delete;
		explicit Armor(bool a_useHandle);
		Armor(const Armor&) = default;
		Armor(Armor&&) = default;
		virtual ~Armor();

		virtual const char*	ClassName() const override;
		virtual void		Clear() override;
		virtual bool		Save(json& a_save) override;
		virtual bool		Load(json& a_load) override;
		virtual FormType	GetType() const override;
		virtual void		Set(TESForm* a_form, BaseExtraList* a_extraList) override;

	protected:
		virtual int			Comp(const ISerializableForm* a_rhs) const override;


		ExtraData _extraData;
	};
}
