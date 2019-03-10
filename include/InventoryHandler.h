#pragma once

#undef AddForm

#include <map>  // map
#include <set>  // set
#include <memory>  // shared_ptr

#include "json.hpp"  // json
#include "Types.h"  // SerializableFormPtr

struct SKSESerializationInterface;


constexpr UInt32 SERIALIZATION_VERSION = 1;


namespace Forms
{
	class InventoryHandler
	{
	public:
		using json = nlohmann::json;
		using key_type = SerializableFormPtr;
		using mapped_type = SInt32;


		static InventoryHandler*	GetSingleton();
		static void					Free();

		void						Clear();
		bool						Save(json& a_save);
		bool						Save(SKSESerializationInterface* a_intfc);
		bool						Load(json& a_load);
		void						AddForm(const key_type& a_form, mapped_type a_count);
		void						UpdateCount(const key_type& a_form, mapped_type a_count);
		void						RemoveFormListByID(UInt32 a_formID);
		void						GarbageCollectoFormList(UInt32 a_formID);

	private:
		struct key_compare
		{
			bool operator()(const key_type& a_lhs, const key_type& a_rhs) const;
		};


		using ItemMap = std::map<key_type, mapped_type, key_compare>;
		using FormList = std::set<key_type, key_compare>;
		using FormMap = std::map<UInt32, FormList>;


		InventoryHandler();
		InventoryHandler(const InventoryHandler&) = delete;
		InventoryHandler(InventoryHandler&&) = delete;
		~InventoryHandler();

		InventoryHandler&	operator=(const InventoryHandler&) = delete;
		InventoryHandler&	operator=(InventoryHandler&&) = delete;


		static constexpr char*	FORMTYPE_STR = "formType";
		static constexpr char*	COUNT_STR = "count";
		static constexpr char*	FORM_STR = "form";

		static InventoryHandler*	_singleton;
		ItemMap						_items;	// key = item, value = total count of that item
		FormMap						_forms;	// key = formID, value = list of all inventory items with that formID
	};
}
