#pragma once

#undef GetForm

#include <type_traits>  // underlying_type_t, is_enum

#include "GameForms.h"  // LookupFormByID
#include "GameObjects.h"  // BGSProjectile
#include "GameData.h"  // DataHandler


namespace iEquip
{
	enum
	{
		kInvalid = 0xFFFFFFFF,

		kSkyrim_WeapTypeBoundArrow = 0x0010D501,

		kUpdate_WAF_WeapTypeGrenade = 0x00AF0237,

		kSpearsBySoolie_Javelin = 0x000073FB,

		kExoticArrows_ccBGSSSE002_ArrowTelekinesisProj01_TriggerExp = 0x00000823,
		kExoticArrows_ccBGSSSE002_ArrowSoulstealerProjectile = 0x00000D64,
		kExoticArrows_ccBGSSSE002_ArrowFireProjectile = 0x00000D68,
		kExoticArrows_ccBGSSSE002_ArrowIceProjectile = 0x00000D69,
		kExoticArrows_ccBGSSSE002_ArrowLightningProjectile = 0x00000D6A,
		kExoticArrows_ccBGSSSE002_ArrowBoneProjectile = 0x00000D67
	};


	template <typename T>
	T* GetForm(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod)
	{
		DataHandler* dataHandler = DataHandler::GetSingleton();
		const ModInfo* info = 0;
		UInt32 formID = a_rawFormID;
		if (a_isLightMod) {
			info = dataHandler->LookupLoadedLightModByName(a_pluginName);
			UInt16 idx = info->lightIndex;
			formID += idx << ((1 * 8) + 4);
			formID += 0xFE << (3 * 8);
		} else {
			info = dataHandler->LookupLoadedModByName(a_pluginName);
			UInt8 idx = info->modIndex;
			formID += idx << (3 * 8);
		}
		TESForm* form = LookupFormByID(formID);
		return (form && form->formType == T::kTypeID) ? static_cast<T*>(form) : 0;
	}


	template <typename T>
	class Form
	{
	public:
		constexpr Form(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod) :
			_rawFormID(a_rawFormID),
			_loadedFormID(kInvalid),
			_pluginName(a_pluginName),
			_isLightMod(a_isLightMod)
		{}


		operator T*()
		{
			T* form = 0;
			if (_loadedFormID == kInvalid) {
				form = GetForm<T>(_rawFormID);
				_loadedFormID = form ? form->formID : kInvalid;
			}

			return form ? form : LookupFormByID(_loadedFormID);
		}


		UInt32 GetLoadedFormID()
		{
			if (_loadedFormID == kInvalid) {
				T* form = GetForm<T>(_rawFormID);
				_loadedFormID = form ? form->formID : kInvalid;
			}
			return _loadedFormID;
		}

	private:
		UInt32		_rawFormID;
		UInt32		_loadedFormID;
		const char*	_pluginName;
		bool		_isLightMod;
	};


	template <typename Enum>
	constexpr auto to_underlying(Enum a_val) noexcept
	{
		return static_cast<std::underlying_type_t<Enum>>(a_val);
	}


	// unary ~operator
	template <typename Enum, typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
	constexpr inline Enum operator ~(Enum a_val)
	{
		a_val = static_cast<Enum>(~static_cast<std::underlying_type_t<Enum>>(a_val));
		return a_val;
	}


	// & operator
	template <typename Enum, typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
	constexpr inline Enum operator &(Enum a_lhs, Enum a_rhs)
	{
		return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a_lhs) & static_cast<std::underlying_type_t<Enum>>(a_rhs));
	}


	// &= operator
	template <typename Enum, typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
	constexpr inline Enum operator &=(Enum& a_lhs, Enum a_rhs)
	{
		a_lhs = static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a_lhs) & static_cast<std::underlying_type_t<Enum>>(a_rhs));
		return a_lhs;
	}


	// | operator
	template <typename Enum, typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
	constexpr inline Enum operator |(Enum a_lhs, Enum a_rhs)
	{
		return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a_lhs) | static_cast<std::underlying_type_t<Enum>>(a_rhs));
	}


	// |= operator
	template <typename Enum, typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
	constexpr inline Enum& operator |=(Enum& a_lhs, Enum a_rhs)
	{
		a_lhs = static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a_lhs) | static_cast<std::underlying_type_t<Enum>>(a_rhs));
		return a_lhs;
	}


	static constexpr const char* NAME_Skyrim = "Skyrim.esm";
	static constexpr const char* NAME_SpearsBySoolie = "SpearsBySoolie.esp";
	static constexpr const char* NAME_Update = "Update.esm";
	static constexpr const char* NAME_ExoticArrows = "ccbgssse002-exoticarrows.esl";
}
