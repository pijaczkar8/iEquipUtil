#include "LocaleManager.h"

#include "GameSettings.h"  // Setting, GetINISetting
#include "GameTypes.h"  // BSFixedString

#include <string>  // wstring
#include <fstream>  // basic_ifstream
#include <map>  // map
#include <queue>  // queue
#include <stack>  // stack
#include <utility>  // pair
#include <codecvt>  // codecvt_mode, codecvt_utf16

#include <wchar.h>  // _wcsicmp
#include <stringapiset.h>  // MultiByteToWideChar, WideCharToMultiByte


bool ci_wstring_compare::operator()(const std::wstring& a_lhs, const std::wstring& a_rhs) const
{
	return _wcsicmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
}


LocaleManager* LocaleManager::GetSingleton()
{
	if (!_singleton) {
		_singleton = new LocaleManager();
	}
	return _singleton;
}


void LocaleManager::Free()
{
	delete _singleton;
}


void LocaleManager::Dump()
{
	std::string key;
	std::string value;
	_DMESSAGE("=== LOCALIZATIONS DUMP BEGIN ===");
	for (auto& pair : GetLocalizationMap()) {
		key = ConvertWStringToString(pair.first);
		value = ConvertWStringToString(pair.second);
		_DMESSAGE("%s: %s", key.c_str(), value.c_str());
	}
	_DMESSAGE("=== LOCALIZATIONS DUMP END ===");
}


void LocaleManager::LoadLocalizationStrings()
{
	constexpr char* PREFIX = "Data\\interface\\translations\\iEquip_";
	constexpr char* ENGLISH = "ENGLISH";
	constexpr char* FILE_EXT = ".txt";

	Setting* language_general = GetINISetting("sLanguage:General");
	std::string path = PREFIX;
	std::string language = (language_general && language_general->GetType() == Setting::kType_String) ? language_general->data.s : ENGLISH;
	bool english = language == ENGLISH;
	path += language;
	path += FILE_EXT;

	ReadFromFile(path.c_str(), english);
	if (!english) {
		path = PREFIX;
		path += ENGLISH;
		path += FILE_EXT;
		ReadFromFile(path.c_str(), true);
	}
}


std::wstring LocaleManager::GetLocalization(std::wstring a_key)
{
	return GetLocalizationInternal(a_key);
}


std::string LocaleManager::GetLocalization(std::string a_key)
{
	return ConvertWStringToString(GetLocalization(ConvertStringToWstring(a_key)));
}


LocaleManager::LocaleManager()
{}


LocaleManager::~LocaleManager()
{}


#pragma warning(push)
#pragma warning(disable : 4996)  // codecvt deprecated in c++17
void LocaleManager::ReadFromFile(const char* a_filePath, bool a_english)
{
	constexpr std::codecvt_mode cvtMode = std::codecvt_mode(std::little_endian | std::consume_header);
	constexpr size_type NPOS = std::wstring::npos;

	LocalizationMap& localizations = a_english ? _localizations_ENG : _localizations_LOC;
	std::wifstream inFile(a_filePath);
	inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, cvtMode>));  // UCS-2 LE w/ BOM
	std::wstring line;
	std::wstring key;
	std::wstring value;
	if (!inFile.is_open()) {
		_ERROR("[ERROR] Failed to open file \"%s\"", a_filePath);
		return;
	}
	while (std::getline(inFile, line)) {
		if (!line.empty() && line.back() == L'\r') {
			line.pop_back();  // discard carriage return
		}
		if (line.empty() || line.front() != L'$') {
			continue;
		}
		key.clear();
		value.clear();

		size_type pos = line.find_first_of(L'{');
		if (pos != NPOS) {
			key = std::wstring(line, 0, pos);
		}

		pos = line.find_first_of(L'\t');
		if (pos != NPOS) {
			if (key.empty()) {
				key = std::wstring(line, 0, pos);
			}
			value = std::wstring(line, pos + 1);
		}

		if (!key.empty() && !value.empty()) {
			localizations.insert(std::make_pair(key, value));
		}
	}
}
#pragma warning(pop)


LocaleManager::LocalizationMap& LocaleManager::GetLocalizationMap()
{
	return _localizations_LOC.empty() ? _localizations_ENG : _localizations_LOC;
}


std::wstring LocaleManager::GetLocalizationInternal(const std::wstring& a_key)
{
	typedef std::wstring::size_type size_t;

	if (a_key[0] != L'$') {
		return a_key;
	}

	std::wstring key(a_key, 0, a_key.find_first_of(L'{'));

	std::stack<size_t> stack;
	std::queue<std::wstring> queue;
	if (!GetNestedLocalizations(a_key, stack, queue)) {
		return a_key;
	}

	std::pair<std::wstring, bool> localization = FindLocalization(key);
	if (!localization.second) {
		return a_key;
	}

	while (!stack.empty()) {
		stack.pop();
	}
	if (InsertLocalizations(localization.first, stack, queue)) {
		for (size_type i = 0, j = 1; j < localization.first.length(); ++i, ++j) {
			if (localization.first[i] == L'\\' && localization.first[j] == L'n') {
				localization.first.replace(i, 2, L"\n");
			}
		}
		return localization.first;
	} else {
		return a_key;
	}
}


bool LocaleManager::GetNestedLocalizations(const std::wstring& a_key, std::stack<size_type>& a_stack, std::queue<std::wstring>& a_queue)
{
	for (size_type pos = 0; pos < a_key.size(); ++pos) {
		switch (a_key[pos]) {
		case L'{':
			a_stack.push(pos);
			break;
		case L'}':
			{
				switch (a_stack.size()) {
				case 0:
					return false;
				case 1:
					{
						size_type last = a_stack.top();
						a_stack.pop();
						size_type off = last + 1;
						size_type count = pos - last - 1;
						std::wstring subStr = (count > 0) ? std::wstring(a_key, off, count) : L"";
						a_queue.push(GetLocalizationInternal(subStr));
					}
					break;
				default:
					a_stack.pop();
				}
				break;
			}
		}
	}

	return true;
}


std::pair<std::wstring, bool> LocaleManager::FindLocalization(const std::wstring& a_key)
{
	LocalizationMap& localizations = GetLocalizationMap();
	auto it = localizations.find(a_key);
	if (it == localizations.end()) {
		if (&localizations != &_localizations_ENG) {
			it = _localizations_ENG.find(a_key);
			if (it == _localizations_ENG.end()) {
				return std::make_pair(L"", false);
			}
		} else {
			return std::make_pair(L"", false);
		}
	}
	return std::make_pair(it->second, true);
}


bool LocaleManager::InsertLocalizations(std::wstring& a_localization, std::stack<size_type>& a_stack, std::queue<std::wstring>& a_queue)
{
	for (size_type pos = 0; pos < a_localization.size(); ++pos) {
		switch (a_localization[pos]) {
		case L'{':
			a_stack.push(pos);
			break;
		case L'}':
			{
				if (a_stack.empty() || a_queue.empty()) {
					return false;
				}

				size_type beg = a_stack.top();
				a_stack.pop();
				std::wstring subStr = a_queue.front();
				a_queue.pop();

				a_localization.replace(beg, pos - beg + 1, subStr);
				pos = beg;
			}
			break;
		}
	}

	return true;
}


std::wstring ConvertStringToWstring(const std::string& a_str)
{
	if (a_str.empty()) {
		return std::wstring();
	}

	int numChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, a_str.c_str(), a_str.length(), NULL, 0);
	std::wstring wstrTo;
	if (numChars) {
		wstrTo.resize(numChars);
		if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, a_str.c_str(), a_str.length(), wstrTo.data(), numChars)) {
			return wstrTo;
		}
	}

	return std::wstring();
}


std::string ConvertWStringToString(const std::wstring& a_str)
{
	if (a_str.empty()) {
		return std::string();
	}

	int numChars = WideCharToMultiByte(CP_UTF8, 0, a_str.c_str(), a_str.length(), NULL, 0, NULL, NULL);
	std::string strTo;
	if (numChars) {
		strTo.resize(numChars);
		if (WideCharToMultiByte(CP_UTF8, 0, a_str.c_str(), a_str.length(), strTo.data(), numChars, NULL, NULL)) {
			return strTo;
		}
	}

	return std::string();
}


LocaleManager* LocaleManager::_singleton = 0;
