#include "InventoryHandler.h"

#include <utility>  // make_pair

#include "Armor.h"  // Armor


namespace iEquip
{
	InventoryHandler* InventoryHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new InventoryHandler();
		}
		return _singleton;
	}


	void InventoryHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	void InventoryHandler::AddForm(const key_type& a_armor, mapped_type a_count)
	{
		auto itemResult = _items.insert(std::make_pair(a_armor, a_count));
		if (itemResult.second && itemResult.first != _items.end()) {
			auto formResult = _forms.insert(std::make_pair(itemResult.first->first->GetLoadedFormID(), FormList()));
			if (formResult.first != _forms.end()) {
				formResult.first->second.insert(a_armor);
			}
		}
	}


	void InventoryHandler::UpdateFormCount(const key_type& a_armor, mapped_type a_countDiff)
	{
		auto itemIt = _items.find(a_armor);
		if (itemIt != _items.end()) {
			itemIt->second += a_countDiff;
			if (itemIt->second <= 0) {
				auto formIt = _forms.find(itemIt->first->GetLoadedFormID());
				if (formIt != _forms.end()) {
					auto formListIt = formIt->second.find(itemIt->first);
					if (formListIt != formIt->second.end()) {
						formIt->second.erase(formListIt);
					}
					if (formIt->second.empty()) {
						_forms.erase(formIt);
					}
				}
				_items.erase(itemIt);
			}
		}
	}


	void InventoryHandler::RemoveFormsByID(UInt32 a_formID)
	{
		auto formIt = _forms.find(a_formID);
		if (formIt != _forms.end()) {
			ItemMap::iterator itemIt;
			for (auto form : formIt->second) {
				itemIt = _items.find(form);
				if (itemIt != _items.end()) {
					_items.erase(itemIt);
				}
			}
			_forms.erase(formIt);
		}
	}


	bool InventoryHandler::key_compare::operator()(const key_type& a_lhs, const key_type& a_rhs) const
	{
		return *a_lhs < *a_rhs;
	}


	InventoryHandler::InventoryHandler()
	{}


	InventoryHandler::~InventoryHandler()
	{}


	InventoryHandler* InventoryHandler::_singleton = 0;
}
