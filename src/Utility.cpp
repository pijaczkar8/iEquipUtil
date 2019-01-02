#include "Utility.h"


namespace iEquip
{
	IForm::IForm(UInt32 a_loadedFormID) :
		_rawFormID(kInvalid),
		_loadedFormID(a_loadedFormID),
		_pluginName(""),
		_isLightMod(false)
	{}


	IForm::IForm(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod) :
		_rawFormID(a_rawFormID),
		_loadedFormID(kInvalid),
		_pluginName(a_pluginName),
		_isLightMod(a_isLightMod)
	{}


	UInt32 IForm::GetLoadedFormID()
	{
		if (_loadedFormID == kInvalid) {
			TESForm* form = GetForm<TESForm>(_rawFormID, _pluginName.c_str(), _isLightMod);
			_loadedFormID = form ? form->formID : kInvalid;
		}
		return _loadedFormID;
	}


	const char* IForm::PluginName() const
	{
		return _pluginName.c_str();
	}


	void ClearLoadedFormIDs()
	{
		WeapTypeBoundArrow.ClearLoadedFormID();
	}


	void LoadForms()
	{
		WeapTypeBoundArrow.GetLoadedFormID();
	}


	Form<BGSKeyword> WeapTypeBoundArrow(kSkyrim_WeapTypeBoundArrow, NAME_Skyrim, false);
}
