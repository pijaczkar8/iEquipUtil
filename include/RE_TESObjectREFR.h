#pragma once

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameForms.h"  // TESForm
#include "ITypes.h"  // UInt32


namespace RE
{
	class TESObjectREFR : public TESForm
	{
	public:
		enum { kTypeID = kFormType_Reference };

		enum RemoveType
		{
			kRemoveType_Remove,
			kRemoveType_Steal,
			kRemoveType_Trade,
			kRemoveType_Drop,
			kRemoveType_Take,
			kRemoveType_Unk05
		};

		virtual void	Unk_39(void);
		virtual void	Unk_3A(void);
		virtual void	Unk_3B(void);
		virtual void	Unk_3C(void);
		virtual void	Unk_3D(void);
		virtual void	Unk_3E(void);
		virtual void	Unk_3F(void);
		virtual void	Unk_40(void);
		virtual void	Unk_41(void);
		virtual void	Unk_42(void);
		virtual void	Unk_43(void);
		virtual void	Unk_44(void);
		virtual void	Unk_45(void);
		virtual void	Unk_46(void);
		virtual void	Unk_47(void);
		virtual void	Unk_48(void);
		virtual void	Unk_49(void);
		virtual void	Unk_4A(void);
		virtual void	Unk_4B(void);
		virtual void	Unk_4C(void);
		virtual void	Unk_4D(void);
		virtual void	Unk_4E(void);
		virtual void	Unk_4F(void);
		virtual void	Unk_50(void);
		virtual void	Unk_51(void);
		virtual void	Unk_52(void);
		virtual void	Unk_53(void);
		virtual void	Unk_54(void);
		virtual void	Unk_55(void);
		virtual void	RemoveItem(UInt32* droppedItemHandle, TESForm* akItem, UInt32 aiCount, UInt32 mode, BaseExtraList* extraList, TESObjectREFR* moveToRef, UInt32 unk7, UInt32 unk8);
	};
};