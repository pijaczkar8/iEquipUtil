#pragma once


namespace RE
{
#if _WIN64
	class ExtraTextDisplayData : public BSExtraData
	{
	public:
		virtual ~ExtraTextDisplayData();

		BSFixedString	name;				// 10
		BGSMessage*		message;			// 18
		TESQuest*		owner;				// 20
		SInt32			unk14;				// 28 -1 default -2 explicit name?
		float			extraHealthValue;	// 2C
		UInt16			rawNameLen;			// 30
		UInt8			pad32[6];			// 32
	};
#else
	class ExtraTextDisplayData : public BSExtraData
	{
	public:
		virtual ~ExtraTextDisplayData();

		BSFixedString	name;				// 08
		BGSMessage*		message;			// 0C
		TESQuest*		owner;				// 10
		SInt32			unk14;				// 14 -1 default -2 explicit name?
		float			extraHealthValue;	// 18
		UInt16			rawNameLen;			// 1C
		UInt16			pad1E;				// 1E
	};
#endif
}
