#pragma once

#undef GetForm

#include <string>  // string
#include <vector>  // vector

#include "GameForms.h"  // LookupFormByID
#include "GameData.h"  // DataHandler


namespace iEquip
{
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


	class Form
	{
		friend class FormFactory;
	public:
		enum
		{
			kInvalid = 0xFFFFFFFF
		};


		bool		IsLightMod() const;
		UInt32		RawFormID() const;
		UInt32		GetLoadedFormID();
		const char*	PluginName() const;

	protected:
		explicit Form(UInt32 a_rawFormID, std::string a_pluginName, bool a_isLightMod);

		void ClearLoadedFormID();


		bool		_isLightMod;
		UInt32		_rawFormID;
		UInt32		_loadedFormID;
		std::string	_pluginName;
	};


	class FormFactory
	{
	public:
		static FormFactory*	GetSingleton();
		static void			Free();
		Form*				CreateForm(UInt32 a_rawFormID, std::string a_pluginName, bool a_isLightMod);
		void				ClearLoadedFormIDs();

	private:
		FormFactory();
		~FormFactory();


		static FormFactory*	_singleton;
		std::vector<Form*>	_forms;
	};
	}
