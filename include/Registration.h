#pragma once

#include "GameReferences.h"  // TESObjectREFR
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // RegistrationSetHolder


class OnPlayerItemAddedRegSet : public RegistrationSetHolder<NullParameters>
{
public:
	static OnPlayerItemAddedRegSet* GetSingleton();
	static void Free();

	void SendEvent(TESForm* a_baseItem, SInt32 a_itemCount, TESObjectREFR* a_itemReference, TESObjectREFR* a_sourceContainer, const BSFixedString& a_name);

private:
	OnPlayerItemAddedRegSet();
	OnPlayerItemAddedRegSet(const OnPlayerItemAddedRegSet&) = delete;
	OnPlayerItemAddedRegSet(OnPlayerItemAddedRegSet&&) = delete;

	OnPlayerItemAddedRegSet& operator=(const OnPlayerItemAddedRegSet&) = delete;
	OnPlayerItemAddedRegSet& operator=(OnPlayerItemAddedRegSet&&) = delete;


	inline static OnPlayerItemAddedRegSet* _singleton = 0;
};
