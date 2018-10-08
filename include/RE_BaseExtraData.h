#pragma once

#include "GameForms.h"  // TESForm
#include "ITypes.h"  // UInt32


namespace RE
{
	class BSExtraData
	{
	public:
		BSExtraData();
		virtual ~BSExtraData();
		virtual UInt32 GetType(void);

		BSExtraData* next;  // 08
		TESForm* form;  // 10

		static BSExtraData* Create(UInt32 size, uintptr_t vtbl);
	};
}