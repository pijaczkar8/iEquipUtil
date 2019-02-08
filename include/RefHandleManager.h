#pragma once

#undef max

#include <cstdlib>  // size_t
#include <bitset>  // bitset
#include <limits>  // numeric_limits
#include <string>  // string
#include <map>  // map

#include "ISerializableData.h"  // ISerializableData
#include "json.hpp"  // json


namespace iEquip
{
	class ISerializableForm;


	class RefHandleManager : public ISerializableData
	{
		friend class RefHandle;

	public:
		using json = nlohmann::json;


		static RefHandleManager*	GetSingleton();
		static void					Free();

		virtual const char*			ClassName() const override;
		virtual void				Clear() override;
		virtual bool				Save(json& a_save) override;
		virtual bool				Load(json& a_load) override;


		ISerializableForm*			LookupFormByHandle(UInt32 a_refHandle);

	protected:
		enum
		{
			kMaxHandles = std::numeric_limits<UInt32>::max() + 1
		};


		RefHandleManager();
		RefHandleManager(const RefHandleManager&) = delete;
		RefHandleManager(RefHandleManager&&) = delete;
		~RefHandleManager();

		RefHandleManager&	operator=(const RefHandleManager&) = delete;
		RefHandleManager&	operator=(RefHandleManager&&) = delete;
		UInt32				GetHandle(ISerializableForm* a_form);
		void				ReturnHandle(UInt32 a_handle);


		static RefHandleManager*				_singleton;
		std::map<UInt32, ISerializableForm*>	_formMap;
		std::bitset<kMaxHandles>				_activeHandles;
		UInt32									_nextHandle;
	};


	template <std::size_t N>
	bool loadJsonObj(nlohmann::json& a_load, const char* a_name, std::bitset<N>& a_val)
	{
		auto& it = a_load.find(a_name);
		if (it != a_load.end()) {
			std::string bits = it.value();
			if (bits.size() != N) {
				_ERROR("[ERROR] Bit string and bit field are of different sizes (%zu vs. %zu)", bits.size(), N);
				return false;
			}

			for (std::size_t i = 0; i < bits.size(); ++i) {
				switch (bits[i]) {
				case '0':
					a_val[i] = false;
					break;
				case '1':
					a_val[i] = true;
					break;
				default:
					_ERROR("[ERROR] Encountered character with no case when assigning bitfield (%c)", bits[i]);
					return false;
				}
			}
			return true;
		} else {
			_ERROR("[ERROR] Could not find (%s) in serialized save data!\n", a_name);
			return false;
		}
	}
}
