#pragma once

#include <string>  // string

#include "IExtraDataType.h"  // IExtraDataType


namespace iEquip
{
	class ExtraTextDisplayData : public IExtraDataType
	{
	public:
		ExtraTextDisplayData();
		ExtraTextDisplayData(const ExtraTextDisplayData&) = default;
		ExtraTextDisplayData(ExtraTextDisplayData&&) = default;
		virtual ~ExtraTextDisplayData();

		virtual const char*		ClassName() const override;
		virtual void			Clear() override;
		virtual bool			Save(json& a_save) override;
		virtual bool			Load(json& a_load) override;
		virtual ExtraDataType	GetType() const override;
		virtual void			Set(const BSExtraData* a_extraData) override;

	protected:
		virtual int				Comp(const IExtraDataType* a_rhs) const override;


		std::string _name;
	};
}
