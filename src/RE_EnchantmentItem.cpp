#include "RE_EnchantmentItem.h"


#include <string>  // TODO


namespace RE
{
	bool operator==(const EnchantmentItem& a_lhs, const EnchantmentItem& a_rhs)
	{
		{
			std::string msg;
			_DMESSAGE("\nLHS");
			msg = "cost == " + std::to_string(a_lhs.data.calculations.cost);
			_DMESSAGE(msg.c_str());
			msg = "flags == " + std::to_string(a_lhs.data.calculations.flags);
			_DMESSAGE(msg.c_str());
			msg = "unk08 == " + std::to_string(a_lhs.data.unk08);
			_DMESSAGE(msg.c_str());
			msg = "amount == " + std::to_string(a_lhs.data.amount);
			_DMESSAGE(msg.c_str());
			msg = "deliveryType == " + std::to_string(a_lhs.data.deliveryType);
			_DMESSAGE(msg.c_str());
			msg = "castingType == " + std::to_string(a_lhs.data.castingType);
			_DMESSAGE(msg.c_str());
			msg = "castTime == " + std::to_string(a_lhs.data.castTime);
			_DMESSAGE(msg.c_str());

			_DMESSAGE("\nRHS");
			msg = "cost == " + std::to_string(a_rhs.data.calculations.cost);
			_DMESSAGE(msg.c_str());
			msg = "flags == " + std::to_string(a_rhs.data.calculations.flags);
			_DMESSAGE(msg.c_str());
			msg = "unk08 == " + std::to_string(a_rhs.data.unk08);
			_DMESSAGE(msg.c_str());
			msg = "amount == " + std::to_string(a_rhs.data.amount);
			_DMESSAGE(msg.c_str());
			msg = "deliveryType == " + std::to_string(a_rhs.data.deliveryType);
			_DMESSAGE(msg.c_str());
			msg = "castingType == " + std::to_string(a_rhs.data.castingType);
			_DMESSAGE(msg.c_str());
			msg = "castTime == " + std::to_string(a_rhs.data.castTime);
			_DMESSAGE(msg.c_str());
		}

		if (a_lhs.data.deliveryType == a_rhs.data.deliveryType &&
			a_lhs.data.castingType == a_rhs.data.castingType &&
			a_lhs.data.castTime == a_rhs.data.castTime)
		{
			bool lhs = (a_lhs.data.baseEnchantment || 0);
			bool rhs = (a_rhs.data.baseEnchantment || 0);
			if (!(lhs ^ rhs)) {
				if (a_lhs.data.baseEnchantment) {
					return (*a_lhs.data.baseEnchantment == *a_rhs.data.baseEnchantment);
				} else {
					return true;
				}
			}
		}
		return false;
	}
}