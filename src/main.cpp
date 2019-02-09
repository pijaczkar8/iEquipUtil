#include "GameEvents.h"  // EventDispatcherList
#include "IDebugLog.h"  // gLog, IDebugLog
#include "PapyrusEvents.h"  // SKSEModCallbackEvent
#include "PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse_version.h"  // RUNTIME_VERSION

#include <clocale>  // setlocale
#include <exception>  // exception
#include <stdexcept>  // runtime_error

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "ActorExt.h"  // RegisterFuncs
#include "AmmoExt.h"  // RegisterFuncs
#include "Events.h"  // EquipEventHandler, InventoryEventHandler, ItemCraftedEventHandler, g_boundWeaponEquippedCallbackRegs, g_boundWeaponUnequippedCallbackRegs
#include "Exceptions.h"  // bad_record_version
#include "FormExt.h"  // RegisterFuncs
#include "Forms.h"  // FormFactory
#include "Hooks.h"  // InstallHooks
#include "InventoryHandler.h"  // InventoryHandler
#include "json.hpp"  // json
#include "LocaleManager.h"  // LocaleManager
#include "RefHandleManager.h"  // RefHandleManager
#include "Settings.h"  // Settings
#include "SoulSeeker.h"  // RegisterFuncs
#include "SpellExt.h"  // RegisterFuncs
#include "StringExt.h"  // RegisterFuncs
#include "version.h"  // IEQUIP_VERSION_VERSTRING, IEQUIP_VERSION_MAJOR
#include "WeaponExt.h"  // RegisterFuncs
#include "RE/GameEvents.h"  // RE::TESEquipEvent
#include "RE/Inventory.h"  // RE::Inventory
#include "RE/ItemCrafted.h"  // RE::ItemCrafted


#if _WIN64
constexpr std::size_t IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_5_62;
constexpr const char* IEQUIP_LOG_PATH = "\\My Games\\Skyrim Special Edition\\SKSE\\iEquipUtil.log";
constexpr const char* IEQUIP_NAME = "iEquipUtil";

#define SINK_EVENT_HANDLER \
RE::EventDispatcherList* eventDispatcherList = reinterpret_cast<RE::EventDispatcherList*>(GetEventDispatcherList()); \
eventDispatcherList->equipDispatcher.AddEventSink(iEquip::EquipEventHandler::GetSingleton())

#else
constexpr std::size_t IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_9_32_0;
constexpr const char* IEQUIP_LOG_PATH = "\\My Games\\Skyrim\\SKSE\\iEquipUtil.log";
constexpr const char* IEQUIP_NAME = "iEquipUtil_LE";

#define SINK_EVENT_HANDLER \
RE::g_equipEventDispatcher->AddEventSink(iEquip::EquipEventHandler::GetSingleton())
#endif


static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface*		g_papyrus = 0;
static SKSEMessagingInterface*		g_messaging = 0;
static SKSESerializationInterface*	g_serialization = 0;

constexpr UInt32 SERIALIZATION_VERSION = 1;


void SaveCallback(SKSESerializationInterface* a_intfc)
{
	using nlohmann::json;
	using iEquip::InventoryHandler;

	InventoryHandler* invHandler = InventoryHandler::GetSingleton();

	try {
		json save;
		if (!invHandler->Save(save)) {
			invHandler->Clear();
			throw std::runtime_error("Inventory handler failed to save data!");
		}

#if _DEBUG
		_DMESSAGE("\nSERIALIZATION SAVE DUMP\n%s\n", save.dump(4).c_str());
#endif
		std::string buf = save.dump();
		g_serialization->WriteRecord('IEQP', SERIALIZATION_VERSION, buf.c_str(), buf.length() + 1);
	} catch (std::exception& e) {
		_ERROR("[ERROR] %s", e.what());
	}

	_MESSAGE("[MESSAGE] Finished saving data");
}


void LoadCallback(SKSESerializationInterface* a_intfc)
{
	using nlohmann::json;
	using iEquip::InventoryHandler;
	using iEquip::RefHandleManager;

	InventoryHandler* invHandler = InventoryHandler::GetSingleton();
	invHandler->Clear();
	RefHandleManager* handleManager = RefHandleManager::GetSingleton();
	handleManager->Clear();

	UInt32 type;
	UInt32 version;
	UInt32 length;
	char* buf = 0;

	try {
		if (!a_intfc->GetNextRecordInfo(&type, &version, &length)) {
			throw std::runtime_error("Serialization interface failed to retrieve next record info!");
		}

		if (version != SERIALIZATION_VERSION) {
			throw bad_record_version(SERIALIZATION_VERSION, version);
		}

		buf = new char[length];
		if (!a_intfc->ReadRecordData(buf, length)) {
			throw std::runtime_error("Serialization interface failed to read record data!");
		}

		json load = json::parse(buf);

#if _DEBUG
		_DMESSAGE("\nSERIALIZATION LOAD DUMP\n%s\n", load.dump(4).c_str());
#endif

		if (!invHandler->Load(load)) {
			invHandler->Clear();
			throw std::runtime_error("Inventory handler failed to load data!");
		}
	} catch (std::exception& e) {
		_ERROR("[ERROR] %s\n", e.what());
	}

	delete buf;
	buf = 0;

	_MESSAGE("[MESSAGE] Finished loading data");
}


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_PreLoadGame:
	case SKSEMessagingInterface::kMessage_DataLoaded:
		{
			RE::Inventory::GetEventSource()->AddEventSink(iEquip::InventoryEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Sinked inventory event handler");

			RE::ItemCrafted::GetEventSource()->AddEventSink(iEquip::ItemCraftedEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Sinked item crafted event handler");

			iEquip::g_boundWeaponEquippedCallbackRegs.Clear();
			iEquip::g_boundWeaponUnequippedCallbackRegs.Clear();
			_DMESSAGE("[DEBUG] Registries cleared");
			iEquip::FormFactory* formFactory = iEquip::FormFactory::GetSingleton();
			formFactory->ClearLoadedFormIDs();
			_DMESSAGE("[DEBUG] Forms cleared");
			iEquip::Settings::OnLoad();
			_DMESSAGE("[DEBUG] Forms loaded");
		}
		break;
	case SKSEMessagingInterface::kMessage_InputLoaded:
		{
			SINK_EVENT_HANDLER;
			iEquip::LocaleManager* locManager = iEquip::LocaleManager::GetSingleton();
			locManager->LoadLocalizationStrings();
			_DMESSAGE("[MESSAGE] Localization strings loaded");
			_DMESSAGE("");
			locManager->Dump();
			_DMESSAGE("");
		}
		break;
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		std::setlocale(LC_ALL, "");

		gLog.OpenRelative(CSIDL_MYDOCUMENTS, IEQUIP_LOG_PATH);
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("%s v%s", IEQUIP_NAME, IEQUIP_VERSION_VERSTRING);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = IEQUIP_NAME;
		a_info->version = IEQUIP_VERSION_MAJOR;

		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}

		if (a_skse->runtimeVersion != IEQUIP_RUNTIME_VER_COMPAT) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] %s loaded", IEQUIP_NAME);

		g_papyrus = (SKSEPapyrusInterface *)a_skse->QueryInterface(kInterface_Papyrus);

		if (iEquip::Settings::loadSettings()) {
			_MESSAGE("[MESSAGE] Settings loaded successfully");
			_DMESSAGE("");
			iEquip::Settings::dump();
			_DMESSAGE("");
		} else {
			_FATALERROR("[FATAL ERROR] Settings failed to load!\n");
			return false;
		}

		bool testActorExt = g_papyrus->Register(iEquip::ActorExt::RegisterFuncs);
		testActorExt ? _MESSAGE("[MESSAGE] iEquip_ActorExt registration successful") : _ERROR("[ERROR] iEquip_ActorExt registration failed!\n");

		bool testAmmoExt = g_papyrus->Register(iEquip::AmmoExt::RegisterFuncs);
		testAmmoExt ? _MESSAGE("[MESSAGE] iEquip_AmmoExt registration successful") : _ERROR("[ERROR] iEquip_AmmoExt registration failed!\n");

		bool testFormExt = g_papyrus->Register(iEquip::FormExt::RegisterFuncs);
		testFormExt ? _MESSAGE("[MESSAGE] iEquip_FormExt registration successful") : _ERROR("[ERROR] iEquip_FormExt registration failed!\n");

		bool testSoulSeeker = g_papyrus->Register(iEquip::SoulSeeker::RegisterFuncs);
		testSoulSeeker ? _MESSAGE("[MESSAGE] iEquip_SoulSeeker registration successful") : _ERROR("[ERROR] iEquip_SoulSeeker registration failed!\n");

		bool testSpellExt = g_papyrus->Register(iEquip::SpellExt::RegisterFuncs);
		testSpellExt ? _MESSAGE("[MESSAGE] iEquip_SpellExt registration successful") : _ERROR("[ERROR] iEquip_SpellExt registration failed!\n");

		bool testStringExt = g_papyrus->Register(iEquip::StringExt::RegisterFuncs);
		testStringExt ? _MESSAGE("[MESSAGE] iEquip_StringExt registration successful") : _ERROR("[ERROR] iEquip_StringExt registration failed!\n");

		bool testWeaponExt = g_papyrus->Register(iEquip::WeaponExt::RegisterFuncs);
		testWeaponExt ? _MESSAGE("[MESSAGE] iEquip_WeaponExt registration successful") : _ERROR("[ERROR] iEquip_WeaponExt registration failed!\n");

		if (!testActorExt || !testAmmoExt || !testFormExt || !testSoulSeeker || !testSpellExt || !testStringExt || !testWeaponExt) {
			_FATALERROR("[FATAL ERROR] Papyrus registration failed!\n");
			return false;
		}

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!\n");
			return false;
		}

		g_serialization = (SKSESerializationInterface*)a_skse->QueryInterface(kInterface_Serialization);
		if (g_serialization) {
			g_serialization->SetUniqueID(g_pluginHandle, 'IEQP');
			g_serialization->SetSaveCallback(g_pluginHandle, SaveCallback);
			g_serialization->SetLoadCallback(g_pluginHandle, LoadCallback);
			_MESSAGE("[MESSAGE] Serialization interface query successful");
		} else {
			_FATALERROR("[FATAL ERROR] Serialization interface query failed!\n");
			return false;
		}

		iEquip::InstallHooks();
		_MESSAGE("[MESSAGE] Installed hooks");

		return true;
	}
};
