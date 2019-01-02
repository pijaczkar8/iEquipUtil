#pragma once

#include "GameForms.h"  // TESForm

#include <string>  // string, stoi
#include <vector>  // vector

#include "Json2Settings.h"  // Json2Settings
#include "Utility.h"  // Form


namespace iEquip
{
	template<>
	class aSetting<Form<TESForm>> :
		public ISetting,
		public std::vector<Form<TESForm>>
	{
	public:
		aSetting() = delete;

		aSetting(std::string a_key, bool a_consoleOK, std::initializer_list<Form<TESForm>> a_list = {}) :
			ISetting(a_key, a_consoleOK),
			std::vector<Form<TESForm>>(a_list)
		{}

		virtual ~aSetting()
		{}

		virtual void assign(json& a_val) override
		{
			clear();
			std::string pluginName;
			std::string formIDStr;
			UInt32 formIDNum;
			bool isLightMod;
			for (auto& val : a_val) {
				val.at("pluginName").get_to(pluginName);
				val.at("formID").get_to(formIDStr);
				formIDNum = stoi(formIDStr, 0, 16);
				val.at("isLightMod").get_to(isLightMod);
				emplace_back(formIDNum, pluginName.c_str(), isLightMod);
			}
		}

		virtual void dump() override
		{
			_DMESSAGE("%s: [", _key.c_str());
			for (auto& it = begin(); it != end(); ++it) {
				_DMESSAGE("{");
				_DMESSAGE("\t\t\"pluginName\": \"%s\",", it->PluginName());
				_DMESSAGE("\t\t\"formID\": \"%u\",", it->RawFormID());
				_DMESSAGE("\t\t\"isLightMod\": \"%s\"", (it->IsLightMod() ? "True" : "False"));
				_DMESSAGE("},");
			}
			_DMESSAGE("]");
		}

		virtual std::string	getValueAsString() const override
		{
			std::string str;
			str = _key + ": [";
			for (auto& it = begin(); it != end(); ++it) {
				str += "{";
				str += "\t\t\"pluginName\": \"";
				str += it->PluginName();
				str += "\",";

				str += "\t\t\"formID\": \"" + std::to_string(it->RawFormID()) + "\",";

				str += "\t\t\"isLightMod\": \"";
				str += it->IsLightMod() ? "True" : "False";
				str += "\"";

				str += "},";
			}
			str += "]";
			return str;
		}
	};


	class Settings : public Json2Settings::Settings
	{
	public:
		Settings() = delete;
		static bool loadSettings(bool a_dumpParse = false);

		static void clearLoadedFormIDs();
		static void loadForms();
		static void sort();


		static aSetting<Form<TESForm>> spears;
		static aSetting<Form<TESForm>> javelins;
		static aSetting<Form<TESForm>> grenades;
		static aSetting<Form<TESForm>> throwingAxes;
		static aSetting<Form<TESForm>> throwingKnives;
		static aSetting<Form<TESForm>> poisonWaxes;
		static aSetting<Form<TESForm>> poisonOils;
		static aSetting<Form<TESForm>> fire;
		static aSetting<Form<TESForm>> ice;
		static aSetting<Form<TESForm>> shock;
		static aSetting<Form<TESForm>> poison;

	private:
		static constexpr char* FILE_NAME = "Data\\SKSE\\Plugins\\iEquip.json";
	};
}
