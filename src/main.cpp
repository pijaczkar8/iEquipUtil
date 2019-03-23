#include "GameEvents.h"  // EventDispatcherList
#include "IDebugLog.h"  // gLog, IDebugLog
#include "PapyrusEvents.h"  // SKSEModCallbackEvent
#include "PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse_version.h"  // RUNTIME_VERSION

#include <clocale>  // setlocale
#include <exception>  // exception
#include <stdexcept>  // runtime_error
#include <vector>  // vector

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "ActorExt.h"  // RegisterFuncs
#include "AmmoExt.h"  // RegisterFuncs
#include "Events.h"  // EquipEventHandler, InventoryEventHandler, ItemCraftedEventHandler, g_boundWeaponEquippedCallbackRegs, g_boundWeaponUnequippedCallbackRegs
#include "Exceptions.h"  // bad_record_version
#include "FormExt.h"  // RegisterFuncs
#include "Forms.h"  // FormFactory
#include "Hooks.h"  // InstallHooks
#include "LocaleManager.h"  // LocaleManager
#include "ObjectReferenceExt.h"  // RegisterFuncs
#include "Registration.h"  // OnPlayerItemAddedRegSet
#include "Settings.h"  // Settings
#include "SoulSeeker.h"  // RegisterFuncs
#include "SpellExt.h"  // RegisterFuncs
#include "StringExt.h"  // RegisterFuncs
#include "UIExt.h"  // RegisterFuncs
#include "version.h"  // IEQUIP_VERSION_VERSTRING, IEQUIP_VERSION_MAJOR
#include "WeaponExt.h"  // RegisterFuncs

#include "RE/TESEquipEvent.h"  // RE::TESEquipEvent


#if _WIN64
constexpr std::size_t IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_5_73;
constexpr const char* IEQUIP_LOG_PATH = "\\My Games\\Skyrim Special Edition\\SKSE\\iEquipUtil.log";
constexpr const char* IEQUIP_NAME = "iEquipUtil";

#define SINK_EVENT_HANDLER \
RE::EventDispatcherList* eventDispatcherList = reinterpret_cast<RE::EventDispatcherList*>(GetEventDispatcherList()); \
eventDispatcherList->equipDispatcher.AddEventSink(Events::EquipEventHandler::GetSingleton())

#else
constexpr std::size_t IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_9_32_0;
constexpr const char* IEQUIP_LOG_PATH = "\\My Games\\Skyrim\\SKSE\\iEquipUtil.log";
constexpr const char* IEQUIP_NAME = "iEquipUtil_LE";

#define SINK_EVENT_HANDLER \
RE::g_equipEventDispatcher->AddEventSink(Events::EquipEventHandler::GetSingleton())
#endif


enum : UInt32
{
	kSerializationVersion = 1,
	kOnPlayerItemAdded = 'OPIA'
};


PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
SKSEPapyrusInterface*		g_papyrus = 0;
SKSEMessagingInterface*		g_messaging = 0;
SKSESerializationInterface*	g_serialization = 0;


void SaveCallback(SKSESerializationInterface* a_intfc)
{
	try {
		OnPlayerItemAddedRegSet::GetSingleton()->Save(a_intfc, kOnPlayerItemAdded, kSerializationVersion);
	} catch (std::exception& e) {
		_ERROR("[ERROR] %s", e.what());
	}

	_MESSAGE("[MESSAGE] Finished saving data");
}


void LoadCallback(SKSESerializationInterface* a_intfc)
{
	try {
		UInt32 type;
		UInt32 version;
		UInt32 length;
		while (a_intfc->GetNextRecordInfo(&type, &version, &length)) {
			switch (type) {
			case kOnPlayerItemAdded:
				{
					OnPlayerItemAddedRegSet* regs = OnPlayerItemAddedRegSet::GetSingleton();
					regs->Clear();
					regs->Load(a_intfc, version);
				}
				break;
			default:
				_ERROR("[ERROR] Invalid record type (%u)!", type);
				break;
			}
		}
	} catch (std::exception& e) {
		_ERROR("[ERROR] %s\n", e.what());
	}

	_MESSAGE("[MESSAGE] Finished loading data");
}


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_PreLoadGame:
	case SKSEMessagingInterface::kMessage_DataLoaded:
		{
			Events::g_boundWeaponEquippedCallbackRegs.Clear();
			Events::g_boundWeaponUnequippedCallbackRegs.Clear();
			_DMESSAGE("[DEBUG] Registries cleared");
			FormFactory* formFactory = FormFactory::GetSingleton();
			formFactory->ClearLoadedFormIDs();
			_DMESSAGE("[DEBUG] Forms cleared");
			Settings::OnLoad();
			_DMESSAGE("[DEBUG] Forms loaded");
		}
		break;
	case SKSEMessagingInterface::kMessage_InputLoaded:
		{
			SINK_EVENT_HANDLER;
			LocaleManager* locManager = LocaleManager::GetSingleton();
			locManager->LoadLocalizationStrings();
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

		g_papyrus = (SKSEPapyrusInterface*)a_skse->QueryInterface(kInterface_Papyrus);

		if (Settings::loadSettings()) {
			_MESSAGE("[MESSAGE] Settings loaded successfully");
		} else {
			_FATALERROR("[FATAL ERROR] Settings failed to load!\n");
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

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!\n");
			return false;
		}

		g_papyrus->Register(ActorExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_ActorExt registration successful");

		g_papyrus->Register(AmmoExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_AmmoExt registration successful");

		g_papyrus->Register(FormExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_FormExt registration successful");

		g_papyrus->Register(ObjectReferenceExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_ObjectReferenceExt registration successful");

		g_papyrus->Register(SoulSeeker::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_SoulSeeker registration successful");

		g_papyrus->Register(SpellExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_SpellExt registration successful");

		g_papyrus->Register(StringExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_StringExt registration successful");

		g_papyrus->Register(UIExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_UIExt registration successful");

		g_papyrus->Register(WeaponExt::RegisterFuncs);
		_MESSAGE("[MESSAGE] iEquip_WeaponExt registration successful");

		InstallHooks();
		_MESSAGE("[MESSAGE] Installed hooks");

		return true;
	}
};
