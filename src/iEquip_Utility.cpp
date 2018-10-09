#include "iEquip_Utility.h"

#include "IErrors.h"  // ASSERT
#include "IFileStream.h"  // IFileStream
#include "ITypes.h"  // UInt32

#include <minwindef.h>  // MAX_PATH
#include <ShlObj.h>  // SHGetFolderPath, CSIDL_LOCAL_APPDATA, SHGFP_TYPE_CURRENT
#include <winerror.h>  // SUCCEEDED

#include <cctype>  // toupper
#include <ios>  // hex
#include <sstream>  // stringstream
#include <string>  // string, strlen


#if _WIN64
constexpr auto SKYRIM_PLUGINS_TXT = "\\Skyrim Special Edition\\plugins.txt";
constexpr auto MOD_INDEX_BEG = 0x04;  // main, update, and DLC
#else
constexpr auto SKYRIM_PLUGINS_TXT = "\\Skyrim\\plugins.txt";
constexpr auto MOD_INDEX_BEG = 0x00;
#endif


namespace iEquip_Utility
{
	bool GIST = false;
	SInt32 GISTIndex = 0x0;


	bool checkForGIST()
	{
		char appdataPath[MAX_PATH];
		ASSERT(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appdataPath)));

		std::string	modlistPath = appdataPath;
		modlistPath += SKYRIM_PLUGINS_TXT;

		// Parse mod list file to acquire GIST presence
		IFileStream modlistFile;
		UInt32 modIndex = MOD_INDEX_BEG;  // main, update, and DLC
		if (modlistFile.Open(modlistPath.c_str())) {
			while (!modlistFile.HitEOF()) {
				char buf[512];
				modlistFile.ReadString(buf, 512, '\n', '\r');

				// skip comments
				if (buf[0] == '#')
					continue;

				// Determine extension type
				std::string line = buf;

#if _WIN64
				// SE: added this
				if (line.length() > 0) {
					if (line.front() != '*')
						continue; // Skip not enabled files

					line = line.substr(1); // Remove the * from name
				}
#endif

				std::string::size_type lastDelim = line.rfind('.');
				if (lastDelim != std::string::npos) {
					std::string ext = line.substr(lastDelim);

					if (_stricmp(ext.c_str(), ".ESM") == 0 || _stricmp(ext.c_str(), ".ESP" ) == 0 || _stricmp(ext.c_str(), ".ESP") == 0) {
						++modIndex;
						std::string name = line.substr(0, lastDelim);
						if (name == "GIST soul trap") {
							GIST = true;
							GISTIndex = modIndex;
							GISTIndex *= 0x1000000;
							break;
						}
					}
				}
			}
		}

		modlistFile.Close();
		return GIST;
	}


	std::string boolToString(bool a_bool)
	{
		return a_bool ? "True" : "False";
	}


	std::string numToHexString(UInt64 a_num, UInt64 a_bytes)
	{
		// Convert to hex
		std::stringstream sstream;
		sstream << std::hex << a_num;
		std::string hexStr = sstream.str();

		// Convert lowercase characters to uppercase
		for (auto& c : hexStr) {
			c = toupper(c);
		}

		// Sign extension
		while (std::strlen(hexStr.c_str()) < a_bytes * 2) {
			hexStr = '0' + hexStr;
		}

		return hexStr;
	}
}