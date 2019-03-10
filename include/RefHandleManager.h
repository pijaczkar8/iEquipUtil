#pragma once

#undef max

#include <cstdlib>  // size_t
#include <bitset>  // bitset
#include <limits>  // numeric_limits
#include <string>  // string
#include <map>  // map

#include "json.hpp"  // json


namespace Forms
{
	class ISerializableForm;


	class RefHandleManager
	{
	public:
		using json = nlohmann::json;


		enum Stat : UInt32
		{
			kMaxHandles = std::numeric_limits<UInt32>::max(),
			kInvalidHandle = kMaxHandles
		};


		static RefHandleManager*	GetSingleton();
		static void					Free();

		void						Clear();
		ISerializableForm*			LookupFormByHandle(UInt32 a_refHandle);
		UInt32						GetHandle(ISerializableForm* a_form);
		void						ReturnHandle(UInt32 a_handle);
		void						SetHandleAsActive(UInt32 a_handle, ISerializableForm* a_form);

	protected:
		RefHandleManager();
		RefHandleManager(const RefHandleManager&) = delete;
		RefHandleManager(RefHandleManager&&) = delete;
		~RefHandleManager();

		RefHandleManager&	operator=(const RefHandleManager&) = delete;
		RefHandleManager&	operator=(RefHandleManager&&) = delete;


		static RefHandleManager*				_singleton;
		std::map<UInt32, ISerializableForm*>	_formMap;
		std::bitset<kMaxHandles>				_activeHandles;
		UInt32									_lastHandle;
	};
}
