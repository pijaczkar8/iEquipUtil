#pragma once

#undef GetForm

#include <type_traits>  // underlying_type_t, is_enum

#include <vector>  // vector

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
#if _WIN64
			info = dataHandler->LookupLoadedLightModByName(a_pluginName);
			if (!info) {
				return 0;
			}
			UInt16 idx = info->lightIndex;
			formID += idx << ((1 * 8) + 4);
			formID += 0xFE << (3 * 8);
#else
			return 0;
#endif
		} else {
#if _WIN64
			info = dataHandler->LookupLoadedModByName(a_pluginName);
#else
			info = dataHandler->LookupModByName(a_pluginName);
#endif
			if (!info) {
				return 0;
			}
			UInt8 idx = info->modIndex;
			formID += idx << (3 * 8);
		}
		TESForm* form = LookupFormByID(formID);
		return (form && form->formType == T::kTypeID) ? static_cast<T*>(form) : 0;
	}


	class IForm
	{
	public:
		IForm(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod);

		constexpr void ClearLoadedFormID()
		{
			_loadedFormID = kInvalid;
		}

	protected:
		UInt32		_rawFormID;
		UInt32		_loadedFormID;
		const char*	_pluginName;
		bool		_isLightMod;
	};


	static std::vector<IForm*>* g_forms;


	void ClearLoadedFormIDs();


	template <typename T>
	class Form : public IForm
	{
	public:
		Form(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod) :
			IForm(a_rawFormID, a_pluginName, a_isLightMod)
		{}


		operator T*()
		{
			T* form = 0;
			if (_loadedFormID == kInvalid) {
				form = GetForm<T>(_rawFormID, _pluginName, _isLightMod);
				_loadedFormID = form ? form->formID : kInvalid;
			}

			return form ? form : LookupFormByID(_loadedFormID);
		}


		UInt32 GetLoadedFormID()
		{
			if (_loadedFormID == kInvalid) {
				T* form = GetForm<T>(_rawFormID, _pluginName, _isLightMod);
				_loadedFormID = form ? form->formID : kInvalid;
			}
			return _loadedFormID;
		}
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

	extern Form<BGSProjectile> ccBGSSSE002_ArrowTelekinesisProj01_TriggerExp;
	extern Form<BGSProjectile> ccBGSSSE002_ArrowSoulstealerProjectile;
	extern Form<BGSProjectile> ccBGSSSE002_ArrowFireProjectile;
	extern Form<BGSProjectile> ccBGSSSE002_ArrowIceProjectile;
	extern Form<BGSProjectile> ccBGSSSE002_ArrowLightningProjectile;
	extern Form<BGSProjectile> ccBGSSSE002_ArrowBoneProjectile;
}
