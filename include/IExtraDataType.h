#pragma once

#include "GameBSExtraData.h"  // ExtraDataType

#include "ISerializableData.h"  // ISerializableData

class BSExtraData;


namespace iEquip
{
	class IExtraDataType : public ISerializableData
	{
	public:
		IExtraDataType();
		IExtraDataType(const IExtraDataType&) = default;
		IExtraDataType(IExtraDataType&&) = default;
		virtual ~IExtraDataType();

		virtual ExtraDataType	GetType() const = 0;
		virtual void			Set(const BSExtraData* a_extraData) = 0;

		bool					operator<(const IExtraDataType& a_rhs) const;
		int						Compare(const IExtraDataType& a_rhs) const;

	protected:
		virtual int				Comp(const IExtraDataType* a_rhs) const = 0;
	};
}
