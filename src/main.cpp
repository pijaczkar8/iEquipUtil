#include "GameEvents.h"  // EventDispatcherList
#include "IDebugLog.h"  // gLog, IDebugLog
#include "PapyrusEvents.h"  // SKSEModCallbackEvent
#include "PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "ActorExt.h"  // RegisterFuncs
#include "AmmoExt.h"  // RegisterFuncs
#include "Events.h"  // g_equipEventHandler, g_boundWeaponEquippedCallbackRegs, g_boundWeaponUnequippedCallbackRegs
#include "FormExt.h"  // RegisterFuncs
#include "Settings.h"  // Settings
#include "SoulSeeker.h"  // RegisterFuncs
#include "Utility.h"  // ClearLoadedFormIDs
#include "version.h"  // IEQUIP_VERSION_VERSTRING, IEQUIP_VERSION_MAJOR
#include "WeaponExt.h"  // RegisterFuncs
#include "RE_GameEvents.h"  // RE::TESEquipEvent


#if _WIN64
constexpr auto IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_5_62;
constexpr auto IEQUIP_LOG_PATH = "\\My Games\\Skyrim Special Edition\\SKSE\\iEquip_SoulSeeker.log";
constexpr auto IEQUIP_NAME = "iEquip_SoulSeeker";

#define SINK_EVENT_HANDLER \
RE::EventDispatcherList* eventDispatcherList = reinterpret_cast<RE::EventDispatcherList*>(GetEventDispatcherList()); \
eventDispatcherList->equipDispatcher.AddEventSink(&iEquip::g_equipEventHandler)

#else
constexpr auto IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_9_32_0;
constexpr auto IEQUIP_LOG_PATH = "\\My Games\\Skyrim\\SKSE\\iEquip_SoulSeeker_LE.log";
constexpr auto IEQUIP_NAME = "iEquip_SoulSeeker_LE";

#define SINK_EVENT_HANDLER \
RE::g_equipEventDispatcher->AddEventSink(&iEquip::g_equipEventHandler)

#endif


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface* g_papyrus = 0;
static SKSEMessagingInterface* g_messaging = 0;


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_PreLoadGame:
		iEquip::g_boundWeaponEquippedCallbackRegs.Clear();
		iEquip::g_boundWeaponUnequippedCallbackRegs.Clear();
		_DMESSAGE("[DEBUG] Registries cleared");
		iEquip::ClearLoadedFormIDs();
		iEquip::Settings::clearLoadedFormIDs();
		_DMESSAGE("[DEBUG] Forms cleared");
		iEquip::LoadForms();
		iEquip::Settings::loadForms();
		iEquip::Settings::sort();
		_DMESSAGE("[DEBUG] Forms loaded");
		break;
	case SKSEMessagingInterface::kMessage_InputLoaded:
	{
		SINK_EVENT_HANDLER;
		break;
	}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
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
			iEquip::Settings::dump();
		} else {
			_FATALERROR("[FATAL ERROR] Settings failed to load!\n");
			return false;
		}

		bool testActorExt = g_papyrus->Register(iEquip::ActorExt::RegisterFuncs);
		testActorExt ? _MESSAGE("[MESSAGE] iEquip_ActorExt registration successful") : _ERROR("[ERROR] iEquip_ActorExt registration failed!");

		bool testAmmoExt = g_papyrus->Register(iEquip::AmmoExt::RegisterFuncs);
		testAmmoExt ? _MESSAGE("[MESSAGE] iEquip_AmmoExt registration successful") : _ERROR("[ERROR] iEquip_AmmoExt registration failed!");

		bool testFormExt = g_papyrus->Register(iEquip::FormExt::RegisterFuncs);
		testFormExt ? _MESSAGE("[MESSAGE] iEquip_FormExt registration successful") : _ERROR("[ERROR] iEquip_FormExt registration failed!");

		bool testSoulSeeker = g_papyrus->Register(iEquip::SoulSeeker::RegisterFuncs);
		testSoulSeeker ? _MESSAGE("[MESSAGE] iEquip_SoulSeeker registration successful") : _ERROR("[ERROR] iEquip_SoulSeeker registration failed!");

		bool testWeaponExt = g_papyrus->Register(iEquip::WeaponExt::RegisterFuncs);
		testWeaponExt ? _MESSAGE("[MESSAGE] iEquip_WeaponExt registration successful") : _ERROR("[ERROR] iEquip_WeaponExt registration failed!");

		if (!testActorExt || !testAmmoExt || !testFormExt || !testSoulSeeker || !testWeaponExt) {
			_FATALERROR("[FATAL ERROR] Papyrus registration failed!\n");
			return false;
		}

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful!\n");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!");
			return false;
		}

		return true;
	}
};
