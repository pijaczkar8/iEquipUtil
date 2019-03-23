#include"Hooks.h"

#if _WIN64
#include "Relocation.h"  // RelocPtr
#endif
#include "SafeWrite.h"  // SafeWrite
#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraTextDisplayData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // TESObjectREFR
#include "GameReferences.h"  // Actor, PlayerCharacter
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "GameTypes.h"  // BSFixedString

#include <cstdint>  // uintptr_t
#include <string>  // string

#include "Registration.h"  // OnPlayerItemAddedRegSet

#include "RE/ExtraTextDisplayData.h"  // ExtraTextDisplayData


namespace
{
	class PlayerCharacterEx : public PlayerCharacter
	{
	public:
		using AddItem_t = void(TESObjectREFR::*)(TESForm* a_item, BaseExtraList* a_extraList, UInt32 a_count, TESObjectREFR* a_fromRefr);
		inline static AddItem_t _AddItem;

		using PickUpItem_t = void(Actor::*)(TESObjectREFR* a_item, UInt32 a_count, bool a_arg3, bool a_playSound);
		inline static PickUpItem_t _PickUpItem;


		void Hook_AddItem(TESForm* a_item, BaseExtraList* a_extraList, UInt32 a_count, TESObjectREFR* a_fromRefr)
		{
			std::string name = "";
			if (a_extraList) {
				RE::ExtraTextDisplayData* xText = static_cast<RE::ExtraTextDisplayData*>(a_extraList->GetByType(kExtraData_TextDisplayData));
				if (xText) {
					name.assign(xText->name.data, xText->rawNameLen);
				}
			}

			if (name.empty()) {
				TESFullName* fullName = DYNAMIC_CAST(a_item, TESForm, TESFullName);
				if (fullName) {
					name = fullName->GetName();
				}
			}

			BSFixedString fixedName(name.c_str());

			(this->*_AddItem)(a_item, a_extraList, a_count, a_fromRefr);

			OnPlayerItemAddedRegSet* regs = OnPlayerItemAddedRegSet::GetSingleton();
			regs->SendEvent(a_item, a_count, 0, a_fromRefr, fixedName);
			CALL_MEMBER_FN(&fixedName, Release)();
		}


		void Hook_PickUpItem(TESObjectREFR* a_item, UInt32 a_count, bool a_arg3, bool a_playSound)
		{
			TESObjectREFR* outRefr = (a_item->flags >> 10) & 1 ? a_item : 0;	// persistent flag

			std::string name = "";
			RE::ExtraTextDisplayData* xText = static_cast<RE::ExtraTextDisplayData*>(a_item->extraData.GetByType(kExtraData_TextDisplayData));
			if (xText) {
				name.assign(xText->name.data, xText->rawNameLen);
			}

			if (name[0] == '\0') {
				TESFullName* fullName = DYNAMIC_CAST(a_item->baseForm, TESForm, TESFullName);
				if (fullName) {
					name = fullName->GetName();
				}
			}

			BSFixedString fixedName(name.c_str());

			(this->*_PickUpItem)(a_item, a_count, a_arg3, a_playSound);

			OnPlayerItemAddedRegSet* regs = OnPlayerItemAddedRegSet::GetSingleton();
			regs->SendEvent(a_item->baseForm, a_count, outRefr, 0, fixedName);
			CALL_MEMBER_FN(&fixedName, Release)();
		}


		static void InstallHooks()
		{
#if _WIN64
			constexpr std::uintptr_t PLAYER_CHARACTER_VTBL = 0x016635F0;	// 1_5_73
#else
			constexpr std::uintptr_t PLAYER_CHARACTER_VTBL = 0x010D201C;
#endif
			constexpr std::uintptr_t PSIZE = sizeof(void*);

			{
#if _WIN64
				RelocPtr<AddItem_t> vFunc(PLAYER_CHARACTER_VTBL + (PSIZE * 0x5A));
				_AddItem = *vFunc;
				SafeWrite64(vFunc.GetUIntPtr(), GetFnAddr(&Hook_AddItem));
#else
				std::uintptr_t addr = PLAYER_CHARACTER_VTBL + (PSIZE * 0x5A);
				_AddItem = *reinterpret_cast<AddItem_t*>(addr);
				SafeWrite32(addr, GetFnAddr(&Hook_AddItem));
#endif
				_MESSAGE("[MESSAGE] Installed add up item hook");
			}

			{
#if _WIN64
				RelocPtr<PickUpItem_t> vFunc(PLAYER_CHARACTER_VTBL + (PSIZE * 0xCC));
				_PickUpItem = *vFunc;
				SafeWrite64(vFunc.GetUIntPtr(), GetFnAddr(&Hook_PickUpItem));
#else
				std::uintptr_t addr = PLAYER_CHARACTER_VTBL + (PSIZE * 0xCB);
				_PickUpItem = *reinterpret_cast<PickUpItem_t*>(addr);
				SafeWrite32(addr, GetFnAddr(&Hook_PickUpItem));
#endif
				_MESSAGE("[MESSAGE] Installed pick up item hook");
			}
		}
	};
}


void InstallHooks()
{
	PlayerCharacterEx::InstallHooks();
}
