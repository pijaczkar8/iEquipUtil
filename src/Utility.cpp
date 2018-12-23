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


	Form<BGSProjectile> ccBGSSSE002_ArrowTelekinesisProj01_TriggerExp(kExoticArrows_ccBGSSSE002_ArrowTelekinesisProj01_TriggerExp, NAME_ExoticArrows, true);
	Form<BGSProjectile> ccBGSSSE002_ArrowSoulstealerProjectile(kExoticArrows_ccBGSSSE002_ArrowSoulstealerProjectile, NAME_ExoticArrows, true);
	Form<BGSProjectile> ccBGSSSE002_ArrowFireProjectile(kExoticArrows_ccBGSSSE002_ArrowFireProjectile, NAME_ExoticArrows, true);
	Form<BGSProjectile> ccBGSSSE002_ArrowIceProjectile(kExoticArrows_ccBGSSSE002_ArrowIceProjectile, NAME_ExoticArrows, true);
	Form<BGSProjectile> ccBGSSSE002_ArrowLightningProjectile(kExoticArrows_ccBGSSSE002_ArrowLightningProjectile, NAME_ExoticArrows, true);
	Form<BGSProjectile> ccBGSSSE002_ArrowBoneProjectile(kExoticArrows_ccBGSSSE002_ArrowBoneProjectile, NAME_ExoticArrows, true);
}
