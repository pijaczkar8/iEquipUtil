#pragma once

#include "AlchemyItem.h"  // AlchemyItem
#include "IExtraDataType.h"  // IExtraDataType


namespace iEquip
{
	class ExtraPoison : public IExtraDataType
	{
	public:
		ExtraPoison();
		ExtraPoison(const ExtraPoison&) = default;
		ExtraPoison(ExtraPoison&&) = default;
		virtual ~ExtraPoison();

		virtual const char*		ClassName() const override;
		virtual void			Clear() override;
		virtual bool			Save(json& a_save) override;
		virtual bool			Load(json& a_load) override;
		virtual ExtraDataType	GetType() const override;
		virtual void			Set(const BSExtraData* a_extraData) override;

	protected:
		virtual int				Comp(const IExtraDataType* a_rhs) const override;


		AlchemyItem	_poison;
		UInt32		_count;
	};
}
