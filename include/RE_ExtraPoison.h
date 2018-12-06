#pragma once

#include "GameBSExtraData.h"  // BSExtraData
#include "GameObjects.h"  // AlchemyItem
#include "ITypes.h"  // UInt32


namespace RE
{
#if _WIN64
	class ExtraPoison : public BSExtraData
	{
	public:
		ExtraPoison();
		virtual ~ExtraPoison();

		// SE: they swapped order of poison and unk
		AlchemyItem*	poison;		// 10
		UInt32			count;		// 18
		UInt32			pad;		// 1C
	};
#else
	class ExtraPoison : public BSExtraData
	{
	public:
		ExtraPoison();
		virtual ~ExtraPoison();

		UInt32			count;		// 08
		AlchemyItem*	poison;		// 0C
	};
#endif
}
