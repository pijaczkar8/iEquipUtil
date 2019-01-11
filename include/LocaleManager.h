#pragma once

#include <map>  // map
#include <queue>  // queue
#include <string>  // wstring, string
#include <locale>  // wstring_convert
#include <codecvt>  // codecvt
#include <cuchar>  // mbstate_t
#include <stack>  // stack
#include <utility>  // pair


namespace iEquip
{
	// https://en.cppreference.com/w/cpp/locale/codecvt
	template<class Facet>
	struct deletable_facet : Facet
	{
		template<class ... Args>
		deletable_facet(Args&& ... a_args) :
			Facet(std::forward<Args>(a_args)...)
		{}

		~deletable_facet() {}
	};


	class ci_wstring_compare
	{
	public:
		bool operator()(const std::wstring& a_lhs, const std::wstring& a_rhs) const;
	};


	class LocaleManager
	{
	public:
		static LocaleManager*	GetSingleton();
		void					Free();
		void					Dump();
		void					LoadLocalizationStrings();
		std::wstring			GetLocalization(std::wstring a_key);
		std::string				GetLocalization(std::string a_key);

	private:
		typedef std::map<std::wstring, std::wstring, ci_wstring_compare> LocalizationMap;
		typedef std::wstring::size_type size_type;


		LocaleManager();
		~LocaleManager();

		void							ReadFromFile(const char* a_filePath, bool a_english);
		LocalizationMap&				GetLocalizationMap();
		std::wstring					GetLocalizationInternal(const std::wstring& a_key);
		bool							GetNestedLocalizations(const std::wstring& a_key, std::stack<size_type>& a_stack, std::queue<std::wstring>& a_queue);
		std::pair<std::wstring, bool>	FindLocalization(const std::wstring& a_key);
		bool							InsertLocalizations(std::wstring& a_localization, std::stack<size_type>& a_stack, std::queue<std::wstring>& a_queue);


		static LocaleManager*																_singleton;
		LocalizationMap																		_localizations_ENG;
		LocalizationMap																		_localizations_LOC;
		std::wstring_convert<deletable_facet<std::codecvt<wchar_t, char, std::mbstate_t>>>	_converter;
	};
}
