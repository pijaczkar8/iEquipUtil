#pragma once

#include <string>  // string, stoi
#include <vector>  // vector
#include <set>  // set

#include "Json2Settings.h"  // Json2Settings
#include "Forms.h"  // Form


namespace iEquip
{
	template<>
	class aSetting<Form*> :
		public ISetting,
		public std::vector<Form*>
	{
	public:
		aSetting() = delete;

		aSetting(std::string a_key, bool a_consoleOK, std::initializer_list<Form*> a_list = {}) :
			ISetting(a_key, a_consoleOK),
			std::vector<Form*>(a_list)
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
			FormFactory* formFactory = FormFactory::GetSingleton();
			Form* form = 0;
			for (auto& val : a_val) {
				val.at("pluginName").get_to(pluginName);
				val.at("formID").get_to(formIDStr);
				formIDNum = stoi(formIDStr, 0, 16);
				val.at("isLightMod").get_to(isLightMod);
				form = formFactory->CreateForm(formIDNum, pluginName, isLightMod);
				push_back(form);
			}
		}

		virtual void dump() override
		{
			Form* form = 0;
			_DMESSAGE("%s: [", _key.c_str());
			for (auto& it = begin(); it != end(); ++it) {
				form = *it;
				_DMESSAGE("{");
				_DMESSAGE("\t\t\"pluginName\": \"%s\",", form->PluginName());
				_DMESSAGE("\t\t\"formID\": \"%u\",", form->RawFormID());
				_DMESSAGE("\t\t\"isLightMod\": \"%s\"", (form->IsLightMod() ? "True" : "False"));
				_DMESSAGE("},");
			}
			_DMESSAGE("]");
		}

		virtual std::string	getValueAsString() const override
		{
			Form* form = 0;
			std::string str;
			str = _key + ": [";
			for (auto& it = begin(); it != end(); ++it) {
				form = *it;

				str += "{";
				str += "\t\t\"pluginName\": \"";
				str += form->PluginName();
				str += "\",";

				str += "\t\t\"formID\": \"" + std::to_string(form->RawFormID()) + "\",";

				str += "\t\t\"isLightMod\": \"";
				str += form->IsLightMod() ? "True" : "False";
				str += "\"";

				str += "},";
			}
			str += "]";
			return str;
		}

		void sort()
		{
			Form* form = 0;
			_loadedFormIDs.clear();
			for (auto& it = begin(); it != end(); ++it) {
				form = *it;
				_loadedFormIDs.insert(form->GetLoadedFormID());
			}
		}

		bool find(UInt32 a_key)
		{
			return _loadedFormIDs.find(a_key) != _loadedFormIDs.end();
		}

	protected:
		std::set<UInt32> _loadedFormIDs;
	};


	class Settings : public Json2Settings::Settings
	{
	public:
		Settings() = delete;
		static bool loadSettings(bool a_dumpParse = false);

		static void OnLoad();


		static aSetting<Form*> spears;
		static aSetting<Form*> javelins;
		static aSetting<Form*> grenades;
		static aSetting<Form*> throwingAxes;
		static aSetting<Form*> throwingKnives;
		static aSetting<Form*> waxes;
		static aSetting<Form*> oils;
		static aSetting<Form*> spellWards;
		static aSetting<Form*> fire;
		static aSetting<Form*> ice;
		static aSetting<Form*> shock;
		static aSetting<Form*> poison;
		static aSetting<Form*> salves;
		static aSetting<Form*> bandages;

	private:
		static constexpr char* FILE_NAME = "Data\\SKSE\\Plugins\\iEquip.json";
	};
}
