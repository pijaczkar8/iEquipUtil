#include "common/IDebugLog.h"  // gLog, IDebugLog
#include "skse64/PluginAPI.h"  // PluginHandle, SKSEPapyrusInterface, SKSEInterface, PluginInfo
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <shlobj.h>  // CSIDL_MYDOCUMENTS

#include "iEquip_SoulSeeker.h"


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface* g_papyrus = 0;


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\iEquip_SoulSeeker.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Warning);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("iEquip_SoulSeeker");

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "iEquip_SoulSeeker";
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor) {
			_MESSAGE("Loaded in editor, marking as incompatible");
			return false;
		} else if (skse->runtimeVersion != RUNTIME_VERSION_1_5_53) {
			_MESSAGE("Unsupported runtime version %08X", skse->runtimeVersion);
			return false;
		}

		// supported runtime version
		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		_MESSAGE("iEquip_SoulSeeker loaded");

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);

		//Check if the function registration was a success...
		bool btest = g_papyrus->Register(iEquip_SoulSeeker::RegisterFuncs);

		if (btest) {
			_MESSAGE("Registery succeeded!");
		} else {
			_MESSAGE("Registry failed!");
		}

		return true;
	}
};
