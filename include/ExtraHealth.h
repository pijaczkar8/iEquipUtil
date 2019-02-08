#pragma once

#include "IExtraDataType.h"  // IExtraDataType


namespace iEquip
{
	class ExtraHealth : public IExtraDataType
	{
	public:
		ExtraHealth();
		ExtraHealth(const ExtraHealth&) = default;
		ExtraHealth(ExtraHealth&&) = default;
		virtual ~ExtraHealth();

		virtual const char*		ClassName() const override;
		virtual void			Clear() override;
		virtual bool			Save(json& a_save) override;
		virtual bool			Load(json& a_load) override;
		virtual ExtraDataType	GetType() const override;
		virtual void			Set(const BSExtraData* a_extraData) override;

	protected:
		virtual int				Comp(const IExtraDataType* a_rhs) const override;


		float _health;
	};
}
