#pragma once

class TESObjectREFR;


namespace RE
{
	struct TESEquipEvent
	{
	public:
		TESObjectREFR*	akSource;		// 00
		UInt32			formID;			// 08
		UInt32			unk0C;			// 0C - formID of a ref handle
		UInt16			uniqueID;		// 10
		bool			isEquipping;	// 12
	};
}
