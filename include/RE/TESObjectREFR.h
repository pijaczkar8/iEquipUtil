#pragma once

#include "GameForms.h"  // TESForm

class BaseExtraList;


namespace RE
{
	class TESObjectREFR : public TESForm
	{
	public:
		enum { kTypeID = kFormType_Reference };


		enum class RemoveType : UInt32
		{
			kRemove,
			kSteal,
			kTrade,
			kDrop,
			kTake,
			kUnk05
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
		virtual UInt32&	RemoveItem(UInt32& a_droppedItemHandle, TESForm* a_item, UInt32 a_count, RemoveType a_mode, BaseExtraList* a_extraList, TESObjectREFR* a_moveToRef, void* a_arg7 = 0, void* a_arg8 = 0);
	};
};
