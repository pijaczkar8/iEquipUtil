#include "skse64/PluginAPI.h"
#include "skse64/GameEvents.h"
#include "skse64/GameMenus.h"
#include "skse64/GameReferences.h"
#include "skse64/ScaleformMovie.h"
#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include <shlobj.h>

#include "SoulGem_Utility.h"


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface* g_papyrus = 0;


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\myTestPlugin.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("SoulGem_Utility");

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "SoulGem_Utility";
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor) {
			_MESSAGE("Loaded in editor, marking as incompatible");
			return false;
		} else if (skse->runtimeVersion != RUNTIME_VERSION_1_5_50) {
			_MESSAGE("Unsupported runtime version %08X", skse->runtimeVersion);
			return false;
		}

		// supported runtime version
		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		_MESSAGE("SoulGem_Utility loaded");

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);

		//Check if the function registration was a success...
		bool btest = g_papyrus->Register(SoulGem_Utility::RegisterFuncs);

		if (btest) {
			_MESSAGE("Registery succeeded");
		}

		return true;
	}
};
