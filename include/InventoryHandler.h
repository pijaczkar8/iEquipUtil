#pragma once

#undef AddForm

#include <map>  // map
#include <set>  // set
#include <memory>  // shared_ptr

#include "json.hpp"  // json
#include "Types.h"  // SerializableFormPtr


namespace iEquip
{
	class InventoryHandler
	{
	public:
		using json = nlohmann::json;


		typedef SerializableFormPtr key_type;
		typedef SInt32 mapped_type;


		static InventoryHandler*	GetSingleton();
		static void					Free();

		void						Clear();
		bool						Save(json& a_save);
		bool						Load(json& a_load);
		void						AddForm(const key_type& a_form, mapped_type a_count);
		void						RemoveFormsByID(UInt32 a_formID);

	private:
		struct key_compare
		{
			bool operator()(const key_type& a_lhs, const key_type& a_rhs) const;
		};


		typedef std::map<key_type, mapped_type, key_compare> ItemMap;
		typedef std::set<key_type> FormList;
		typedef std::map<UInt32, FormList> FormMap;


		InventoryHandler();
		InventoryHandler(const InventoryHandler&) = delete;
		InventoryHandler(InventoryHandler&&) = delete;
		~InventoryHandler();

		InventoryHandler&	operator=(const InventoryHandler&) = delete;
		InventoryHandler&	operator=(InventoryHandler&&) = delete;


		static constexpr char*	KEY_STR = "count";
		static constexpr char*	VALUE_STR = "form";

		static InventoryHandler*	_singleton;
		ItemMap						_items;	// key = item, value = total count of that item
		FormMap						_forms;	// key = formID, value = list of all inventory items with that formID
	};
}
