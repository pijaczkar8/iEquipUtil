#include "GameEvents.h"  // EventDispatcherList
#include "IDebugLog.h"  // gLog, IDebugLog
#include "PapyrusEvents.h"  // SKSEModCallbackEvent
#include "PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS
#include <string>  // string

#include "iEquip_ActorExt.h"  // RegisterFuncs
#include "iEquip_AmmoExt.h"  // RegisterFuncs
#include "iEquip_Events.h"  // g_equipEventHandler, g_callbackDispatcher
#include "iEquip_FormExt.h"  // RegisterFuncs
#include "iEquip_SoulSeeker.h"  // RegisterFuncs
#include "iEquip_WeaponExt.h"  // RegisterFuncs
#include "RE_GameEvents.h"  // RE::TESEquipEvent


#if _WIN64
constexpr auto IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_5_50;
constexpr auto IEQUIP_LOG_PATH = "\\My Games\\Skyrim Special Edition\\SKSE\\iEquip_SoulSeeker.log";
constexpr auto IEQUIP_NAME = "iEquip_SoulSeeker";

#define SINK_EVENT_HANDLER \
RE::EventDispatcherList* eventDispatcherList = reinterpret_cast<RE::EventDispatcherList*>(GetEventDispatcherList()); \
eventDispatcherList->equipDispatcher.AddEventSink(&iEquip_Events::g_equipEventHandler)

#else
constexpr auto IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_9_32_0;
constexpr auto IEQUIP_LOG_PATH = "\\My Games\\Skyrim\\SKSE\\iEquip_SoulSeeker_LE.log";
constexpr auto IEQUIP_NAME = "iEquip_SoulSeeker_LE";

#define SINK_EVENT_HANDLER \
RE::g_equipEventDispatcher->AddEventSink(&iEquip_Events::g_equipEventHandler)

#endif


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface* g_papyrus = 0;
static SKSEMessagingInterface* g_messaging = 0;


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_PreLoadGame:
		iEquip_Events::g_callbackRegs.Clear();
		_DMESSAGE("[DEBUG] Registry cleared\n");
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

		_MESSAGE(IEQUIP_NAME);

		// populate info structure
		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = IEQUIP_NAME;
		a_info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!");
			return false;
		} else if (a_skse->runtimeVersion != IEQUIP_RUNTIME_VER_COMPAT) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!", a_skse->runtimeVersion);
			return false;
		}

		// supported runtime version
		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		std::string msg = "[MESSAGE] ";
		msg += IEQUIP_NAME;
		msg += " loaded";
		_MESSAGE(msg.c_str());

		g_papyrus = (SKSEPapyrusInterface *)a_skse->QueryInterface(kInterface_Papyrus);

		bool testActorExt = g_papyrus->Register(iEquip_ActorExt::RegisterFuncs);
		testActorExt ? _MESSAGE("[MESSAGE] iEquip_ActorExt registration successful!") : _ERROR("[ERROR] iEquip_ActorExt registration failed!");

		bool testAmmoExt = g_papyrus->Register(iEquip_AmmoExt::RegisterFuncs);
		testAmmoExt ? _MESSAGE("[MESSAGE] iEquip_AmmoExt registration successful!") : _ERROR("[ERROR] iEquip_AmmoExt registration failed!");

		bool testFormExt = g_papyrus->Register(iEquip_FormExt::RegisterFuncs);
		testFormExt ? _MESSAGE("[MESSAGE] iEquip_FormExt registration successful!") : _ERROR("[ERROR] iEquip_FormExt registration failed!");

		bool testSoulSeeker = g_papyrus->Register(iEquip_SoulSeeker::RegisterFuncs);
		testSoulSeeker ? _MESSAGE("[MESSAGE] iEquip_SoulSeeker registration successful!") : _ERROR("[ERROR] iEquip_SoulSeeker registration failed!");

		bool testWeaponExt = g_papyrus->Register(iEquip_WeaponExt::RegisterFuncs);
		testWeaponExt ? _MESSAGE("[MESSAGE] iEquip_WeaponExt registration successful!") : _ERROR("[ERROR] iEquip_WeaponExt registration failed!");

		if (!testActorExt || !testAmmoExt || !testFormExt || !testSoulSeeker || !testWeaponExt) {
			_FATALERROR("[FATAL ERROR] Papyrus registration failed!");
			return false;
		}

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful!");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!");
			return false;
		}

		iEquip_Events::g_callbackDispatcher = (EventDispatcher<SKSEModCallbackEvent>*)g_messaging->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_ModEvent);
		if (iEquip_Events::g_callbackDispatcher) {
			_MESSAGE("[MESSAGE] Mod callback event dispatcher query successful!\n");
		} else {
			_FATALERROR("[FATAL ERROR] Mod callback event dispatcher query failed!");
			return false;
		}

		return true;
	}
};
