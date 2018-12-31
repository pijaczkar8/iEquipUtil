#include "Settings.h"


namespace iEquip
{
	bool Settings::loadSettings(bool a_dumpParse)
	{
		Json2Settings::Settings::setFileName(FILE_NAME);
		return Json2Settings::Settings::loadSettings(a_dumpParse);
	}


	aSetting<Form<TESForm>> Settings::spears("spears", false);
	aSetting<Form<TESForm>>	Settings::javelins("javelins", false);
	aSetting<Form<TESForm>> Settings::grenades("grenades", false);
	aSetting<Form<TESForm>> Settings::throwingAxes("throwingAxes", false);
	aSetting<Form<TESForm>> Settings::throwingKnives("throwingKnives", false);
	aSetting<Form<TESForm>> Settings::poisonWaxes("poisonWaxes", false);
	aSetting<Form<TESForm>> Settings::poisonOils("poisonOils", false);
	aSetting<Form<TESForm>> Settings::fire("fire", false);
	aSetting<Form<TESForm>> Settings::ice("ice", false);
	aSetting<Form<TESForm>> Settings::shock("shock", false);
	aSetting<Form<TESForm>> Settings::poison("poison", false);
}
