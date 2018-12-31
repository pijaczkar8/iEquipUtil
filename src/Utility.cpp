#include "Utility.h"


namespace iEquip
{
	IForm::IForm(UInt32 a_rawFormID, const char* a_pluginName, bool a_isLightMod) :
		_rawFormID(a_rawFormID),
		_loadedFormID(kInvalid),
		_pluginName(a_pluginName),
		_isLightMod(a_isLightMod)
	{
		if (!g_forms) {
			g_forms = new std::vector<IForm*>;
		}
		g_forms->push_back(this);
	}


	void ClearLoadedFormIDs()
	{
		if (g_forms) {
			for (auto& form : *g_forms) {
				form->ClearLoadedFormID();
			}
		}
	}


	void LoadForms()
	{
		if (g_forms) {
			for (auto& form : *g_forms) {
				form->GetLoadedFormID();
			}
		}
	}


	Form<BGSKeyword> WeapTypeBoundArrow(kSkyrim_WeapTypeBoundArrow, NAME_Skyrim, false);
}
