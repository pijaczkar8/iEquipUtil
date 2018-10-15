#include "GameEvents.h"  // BSTEventSink, EventResult, EventDispatcher
#include "GameForms.h"  // TESQuest
#include "GameTypes.h"  // BSFixedString
#include "IDebugLog.h"  // gLog, IDebugLog
#include "PapyrusEvents.h"  // SKSEModCallbackEvent, g_modCallbackEventDispatcher
#include "PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS
#include <string>  // string

#include "iEquip_ActorExt.h"  // RegisterFuncs
#include "iEquip_FormExt.h"  // registry, RegisterFuncs
#include "iEquip_SoulSeeker.h"  // RegisterFuncs
#include "iEquip_WeaponExt.h"  // RegisterFuncs
#include "RE_GameEvents.h"  // RE::TESEquipEvent


#include <sstream>  // TODO


#if _WIN64
constexpr auto IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_5_50;
constexpr auto IEQUIP_LOG_PATH = "\\My Games\\Skyrim Special Edition\\SKSE\\iEquip_SoulSeeker.log";
constexpr auto IEQUIP_NAME = "iEquip_SoulSeeker";
#else
constexpr auto IEQUIP_RUNTIME_VER_COMPAT = RUNTIME_VERSION_1_9_32_0;
constexpr auto IEQUIP_LOG_PATH = "\\My Games\\Skyrim\\SKSE\\iEquip_SoulSeeker_LE.log";
constexpr auto IEQUIP_NAME = "iEquip_SoulSeeker_LE";
#endif


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface* g_papyrus = 0;
static SKSEMessagingInterface* g_messaging = 0;


class EquipEventHandler : public BSTEventSink<RE::TESEquipEvent>
{
public:
	virtual EventResult ReceiveEvent(RE::TESEquipEvent* a_evn, EventDispatcher<RE::TESEquipEvent>* a_dispatcher)
	{
		if (a_evn->akSource != *g_thePlayer) {
			return kEvent_Continue;
		} else if (a_evn->checkIfBoundWeapEquipped()) {
			static BSFixedString eventName = "BoundWeaponEquipped";
			for (auto& form : iEquip_FormExt::registry) {
				// TODO
				std::stringstream ss;
				ss << form;
				_DMESSAGE(ss.str().c_str());
				// TODO

				SKSEModCallbackEvent evn(eventName, "", 0.0, form);
				g_modCallbackEventDispatcher.SendEvent(&evn);
				_DMESSAGE("BoundWeaponEquipped event dispatched\n");
			}
		}
		return kEvent_Continue;
	}
};


EquipEventHandler g_equipEventHandler;


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_PreLoadGame:
		//iEquip_QuestExt::registry.clear();
		//_DMESSAGE("registry cleared\n");
		break;
	case SKSEMessagingInterface::kMessage_InputLoaded:
	{
		EventDispatcherList* tmpEventDispatcherList = GetEventDispatcherList();
		RE::EventDispatcherList* eventDispatcherList = reinterpret_cast<RE::EventDispatcherList*>(tmpEventDispatcherList);
		eventDispatcherList->equipDispatcher.AddEventSink(&g_equipEventHandler);
		break;
	}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, IEQUIP_LOG_PATH);
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE(IEQUIP_NAME);

		// populate info structure
		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = IEQUIP_NAME;
		a_info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("FATAL ERROR: Loaded in editor, marking as incompatible!");
			return false;
		} else if (a_skse->runtimeVersion != IEQUIP_RUNTIME_VER_COMPAT) {
			_FATALERROR("FATAL ERROR: Unsupported runtime version %08X!", a_skse->runtimeVersion);
			return false;
		}

		// supported runtime version
		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		std::string msg = IEQUIP_NAME;
		msg += " loaded";
		_MESSAGE(msg.c_str());

		g_papyrus = (SKSEPapyrusInterface *)a_skse->QueryInterface(kInterface_Papyrus);

		bool testActorExt = g_papyrus->Register(iEquip_ActorExt::RegisterFuncs);
		testActorExt ? _DMESSAGE("iEquip_ActorExt registration succeeded!") : _DMESSAGE("iEquip_ActorExt registration failed!");

		bool testFormExt = g_papyrus->Register(iEquip_FormExt::RegisterFuncs);
		testFormExt ? _DMESSAGE("iEquip_FormExt registration succeeded!") : _DMESSAGE("iEquip_FormExt registration failed!");

		bool testSoulSeeker = g_papyrus->Register(iEquip_SoulSeeker::RegisterFuncs);
		testSoulSeeker ? _DMESSAGE("iEquip_SoulSeeker registration succeeded!") : _DMESSAGE("iEquip_SoulSeeker registration failed!");

		bool testWeaponExt = g_papyrus->Register(iEquip_WeaponExt::RegisterFuncs);
		testWeaponExt ? _DMESSAGE("iEquip_WeaponExt registration succeeded!") : _DMESSAGE("iEquip_WeaponExt registration failed!");

		if (!testSoulSeeker || !testFormExt || !testActorExt || !testWeaponExt) {
			_FATALERROR("FATAL ERROR: Papyrus registration failed!");
			return false;
		}

		g_messaging = (SKSEMessagingInterface *)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("Messaging interface registration succeeded!\n");
		} else {
			_FATALERROR("FATAL ERROR: Messaging interface registration failed!");
			return false;
		}

		return true;
	}
};
