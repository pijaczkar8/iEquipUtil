#include "Settings.h"


namespace iEquip
{
	bool Settings::loadSettings(bool a_dumpParse)
	{
		Json2Settings::Settings::setFileName(FILE_NAME);
		return Json2Settings::Settings::loadSettings(a_dumpParse);
	}


	void Settings::OnLoad()
	{
		if (settings) {
			for (auto& setting : *settings) {
				aSetting<Form*>* arr = dynamic_cast<aSetting<Form*>*>(setting);
				if (arr) {
					arr->sort();
				}
			}
		}
	}


	aSetting<Form*>	Settings::spears("spears", false);
	aSetting<Form*>	Settings::javelins("javelins", false);
	aSetting<Form*>	Settings::grenades("grenades", false);
	aSetting<Form*>	Settings::throwingAxes("throwingAxes", false);
	aSetting<Form*>	Settings::throwingKnives("throwingKnives", false);
	aSetting<Form*>	Settings::waxes("waxes", false);
	aSetting<Form*>	Settings::oils("oils", false);
	aSetting<Form*>	Settings::spellWards("spellWards", false);
	aSetting<Form*>	Settings::fire("fire", false);
	aSetting<Form*>	Settings::ice("ice", false);
	aSetting<Form*>	Settings::shock("shock", false);
	aSetting<Form*>	Settings::poison("poison", false);
	aSetting<Form*>	Settings::salves("salves", false);
	aSetting<Form*>	Settings::bandages("bandages", false);
}
