#include "IDebugLog.h"  // gLog, IDebugLog
#include "ITypes.h"  // UInt32
#include "PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS
#include <string>  // string

#include "iEquip_ActorExt.h"  // RegisterFuncs
#include "iEquip_SoulSeeker.h"  // RegisterFuncs
#include "iEquip_SoulSeekerLib.h"  // gemUtil
#include "iEquip_Utility.h"  // checkForGIST
#include "iEquip_WeaponExt.h"  // RegisterFuncs


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
			_FATALERROR("Loaded in editor, marking as incompatible");
			return false;
		} else if (a_skse->runtimeVersion != IEQUIP_RUNTIME_VER_COMPAT) {
			_FATALERROR("Unsupported runtime version %08X", a_skse->runtimeVersion);
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

		//Check if the function registration was a success...
		bool testActorExt = g_papyrus->Register(iEquip_ActorExt::RegisterFuncs);
		bool testSoulSeeker = g_papyrus->Register(iEquip_SoulSeeker::RegisterFuncs);
		bool testWeaponExt = g_papyrus->Register(iEquip_WeaponExt::RegisterFuncs);

		if (testSoulSeeker && testActorExt && testWeaponExt) {
			_MESSAGE("Papyrus registration succeeded!\n");
		} else {
			_ERROR("Papyrus registration failed!");
			return false;
		}

		return true;
	}
};
