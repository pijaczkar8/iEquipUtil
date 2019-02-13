#include "ISerializableForm.h"

#include "GameData.h"  // DataHandler, ModInfo

#include <exception>  // exception

#include "version.h"  // MAKE_STR


namespace iEquip
{
	ISerializableForm::ISerializableForm(bool a_useHandle) :
		ISerializableData(),
		_rawFormID(kInvalid),
		_loadedFormID(kInvalid),
		_pluginName(""),
		_isLightMod(false),
		_isGeneratedID(false),
		_refHandle(a_useHandle, this)
	{}


	ISerializableForm::~ISerializableForm()
	{}


	void ISerializableForm::Clear()
	{
		_rawFormID = kInvalid;
		_loadedFormID = kInvalid;
		_pluginName = "";
		_isLightMod = false;
		_isGeneratedID = false;
	}


	bool ISerializableForm::Save(json& a_save)
	{
		try {
			json refHandleSave;
			if (!_refHandle.Save(refHandleSave)) {
				return false;
			}

			a_save = {
				{ MAKE_STR(_rawFormID), _rawFormID },
				{ MAKE_STR(_pluginName), _pluginName },
				{ MAKE_STR(_isLightMod), _isLightMod },
				{ MAKE_STR(_isGeneratedID), _isGeneratedID },
				{ _refHandle.ClassName(), refHandleSave }
			};
		} catch (std::exception & e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool ISerializableForm::Load(json& a_load)
	{
		try {
			auto it = a_load.find(_refHandle.ClassName());
			if (it == a_load.end() || !_refHandle.Load(*it)) {
				return false;
			}

			if (!loadJsonObj(a_load, MAKE_STR(_rawFormID), _rawFormID)) {
				return false;
			}

			_loadedFormID = kInvalid;

			if (!loadJsonObj(a_load, MAKE_STR(_pluginName), _pluginName)) {
				return false;
			}

			if (!loadJsonObj(a_load, MAKE_STR(_isLightMod), _isLightMod)) {
				return false;
			}

			if (!loadJsonObj(a_load, MAKE_STR(_isGeneratedID), _isGeneratedID)) {
				return false;
			}
		} catch (std::exception & e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	void ISerializableForm::Set(TESForm* a_form, BaseExtraList* a_extraList)
	{}


	bool ISerializableForm::operator<(const ISerializableForm& a_rhs) const
	{
		return Compare(a_rhs) == -1;
	}


	int ISerializableForm::Compare(const ISerializableForm& a_rhs) const
	{
		int result = GetType() - a_rhs.GetType();
		if (result != 0) {
			if (result < 0) {
				return -1;
			} else {
				return 1;
			}
		} else {
			return Comp(&a_rhs);
		}
	}


	TESForm* ISerializableForm::GetForm() const
	{
		UInt32 formID = GetLoadedFormID();
		return formID == kInvalid ? 0 : LookupFormByID(formID);
	}


	void ISerializableForm::SetForm(UInt32 a_formID)
	{
		if (a_formID == _loadedFormID) {
			return;
		}

		UInt32 rawFormID = a_formID;
		UInt8 idx = (a_formID >> (3 * 8)) & 0xFF;
		const ModInfo * modInfo = 0;
		bool isLightMod = idx == 0xFE;
		bool isGeneratedID = idx == 0xFF;
		if (isLightMod) {
			UInt16 lightIdx = (a_formID >> ((1 * 8) + 4)) & 0xFFF;
			modInfo = LookupLoadedLightModByIndex(lightIdx);
			rawFormID &= 0xFFF;
		} else {
			if (!isGeneratedID) {
				modInfo = LookupLoadedModByIndex(idx);
				rawFormID &= 0xFFFFFF;
			}
		}

		if (isGeneratedID || modInfo) {
			_pluginName = modInfo ? modInfo->name : "Skyrim.esm";
			_rawFormID = rawFormID;
			_loadedFormID = a_formID;
			_isLightMod = isLightMod;
			_isGeneratedID = isGeneratedID;
		}
	}


	UInt32 ISerializableForm::GetRawFormID() const
	{
		return _rawFormID;
	}


	UInt32 ISerializableForm::GetLoadedFormID() const
	{
		if (_rawFormID == kInvalid) {
			return kInvalid;
		}

		if (_loadedFormID == kInvalid) {
			ISerializableForm* form = const_cast<ISerializableForm*>(this);
			DataHandler* dataHandler = DataHandler::GetSingleton();
			const ModInfo* modInfo = 0;
			if (_isLightMod) {
#if _WIN64
				modInfo = dataHandler->LookupLoadedLightModByName(_pluginName.c_str());
				if (!modInfo) {
					form->_rawFormID = kInvalid;
					return kInvalid;
				}
				form->_loadedFormID = _rawFormID;
				form->_loadedFormID += modInfo->lightIndex << ((1 * 8) + 4);
				form->_loadedFormID += 0xFE << (3 * 8);
#else
				form->_rawFormID = kInvalid;
#endif
			} else {
				if (_isGeneratedID) {
					form->_loadedFormID = _rawFormID;
					form->_loadedFormID += 0xFF << (3 * 8);
				} else {
#if _WIN64
					modInfo = dataHandler->LookupLoadedModByName(_pluginName.c_str());
#else
					modInfo = dataHandler->LookupModByName(_pluginName.c_str());
#endif
					if (!modInfo) {
						form->_rawFormID = kInvalid;
						return kInvalid;
					}
					form->_loadedFormID = _rawFormID;
					form->_loadedFormID += modInfo->modIndex << (3 * 8);
				}
			}
		}

		return _loadedFormID;
	}


	UInt32 ISerializableForm::GetHandle() const
	{
		return _refHandle.GetHandle();
	}


	const ModInfo* ISerializableForm::LookupLoadedModByIndex(UInt8 a_index)
	{
		DataHandler* dataHandler = DataHandler::GetSingleton();
#if _WIN64
		UInt32 size = dataHandler->modList.loadedMods.count;
#else
		UInt32 size = 0xFF;
#endif
		for (UInt32 i = 0; i < size; ++i) {
			if (dataHandler->modList.loadedMods[i]->modIndex == a_index) {
				return dataHandler->modList.loadedMods[i];
			}
		}
		return 0;
	}


	const ModInfo* ISerializableForm::LookupLoadedLightModByIndex(UInt16 a_index)
	{
#if _WIN64
		DataHandler* dataHandler = DataHandler::GetSingleton();
		for (UInt32 i = 0; i < dataHandler->modList.loadedCCMods.count; ++i) {
			if (dataHandler->modList.loadedCCMods[i]->lightIndex == a_index) {
				return dataHandler->modList.loadedCCMods[i];
			}
		}
#endif
		return 0;
	}
}
