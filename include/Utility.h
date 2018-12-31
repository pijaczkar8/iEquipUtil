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
		kSkyrim_WeapTypeBoundArrow = 0x0010D501
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
		return static_cast<T*>(form);
	}


	class IForm
	{
	public:
		IForm(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod);

		UInt32 GetLoadedFormID()
		{
			if (_loadedFormID == kInvalid) {
				TESForm* form = GetForm<TESForm>(_rawFormID, _pluginName.c_str(), _isLightMod);
				_loadedFormID = form ? form->formID : kInvalid;
			}
			return _loadedFormID;
		}

		constexpr void ClearLoadedFormID()
		{
			_loadedFormID = kInvalid;
		}

		constexpr UInt32 RawFormID() const
		{
			return _rawFormID;
		}

		const char* PluginName() const
		{
			return _pluginName.c_str();
		}

		constexpr bool IsLightMod() const
		{
			return _isLightMod;
		}

	protected:
		UInt32		_rawFormID;
		UInt32		_loadedFormID;
		std::string	_pluginName;
		bool		_isLightMod;
	};


	static std::vector<IForm*>* g_forms;


	void ClearLoadedFormIDs();
	void LoadForms();


	template <typename T>
	class Form : public IForm
	{
	public:
		Form(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod) :
			IForm(a_rawFormID, a_pluginName, a_isLightMod)
		{}


		operator T*()
		{
			if (_loadedFormID == kInvalid) {
				GetLoadedFormID();
			}
			return static_cast<T*>(LookupFormByID(_loadedFormID));
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
	static constexpr const char* NAME_Update = "Update.esm";

	extern Form<BGSKeyword> WeapTypeBoundArrow;
}
