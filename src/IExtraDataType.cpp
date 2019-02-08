#include "IExtraDataType.h"


namespace iEquip
{
	IExtraDataType::IExtraDataType() :
		ISerializableData()
	{}


	IExtraDataType::~IExtraDataType()
	{}


	bool IExtraDataType::operator<(const IExtraDataType& a_rhs) const
	{
		return Compare(a_rhs) < 0;
	}


	int IExtraDataType::Compare(const IExtraDataType& a_rhs) const
	{
		int result = GetType() - a_rhs.GetType();
		if (result != 0) {
			return result < 0 ? -1 : 1;
		} else {
			return Comp(&a_rhs);
		}
	}
}
