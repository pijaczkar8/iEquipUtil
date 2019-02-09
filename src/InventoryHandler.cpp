#include "InventoryHandler.h"

#include <utility>  // make_pair
#include <string>  // to_string, stoi

#include "ISerializableForm.h"  // ISerializableForm
#include "SerializableFormFactory.h"  // Serializable::FormFactory


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


	void InventoryHandler::Clear()
	{
		_items.clear();
		_forms.clear();
	}


	bool InventoryHandler::Save(json& a_save)
	{
		try {
			json formSave;
			json itemSave;
			for (auto& item : _items) {
				if (item.first->Save(formSave)) {
					itemSave = {
						{ KEY_STR, item.second },
						{ VALUE_STR, formSave }
					};
					a_save[std::to_string(item.first->GetType())].push_back(itemSave);
				}
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool InventoryHandler::Load(json& a_load)
	{
		try {
			Serializable::FormFactory* formFactory = Serializable::FormFactory::GetSingleton();
			SerializableFormPtr form;
			UInt32 formType;
			UInt32 count;
			for (auto& typeToKVList : a_load.items()) {
				formType = std::stoi(typeToKVList.key());
				for (auto& kvPair : typeToKVList.value()) {
					form = formFactory->GetForm(formType, false);
					if (form) {
						auto it = kvPair.find(KEY_STR);
						if (it != kvPair.end()) {
							count = it.value();
						} else {
							throw std::runtime_error("Failed to find key in kv pair!");
						}

						it = kvPair.find(VALUE_STR);
						if (it != kvPair.end()) {
							if (form->Load(it.value())) {
								auto formResult = _forms.insert({ form->GetLoadedFormID(), FormList() });
								if (formResult.first != _forms.end()) {
									formResult.first->second.insert(form);	// push form into form list
								} else {
									throw std::runtime_error("Failed to insert formlist into form map!");
								}

								auto itemResult = _items.insert({ form, count });
								if (itemResult.first != _items.end()) {
									if (!itemResult.second) {
										itemResult.first->second += count;
									}
								} else {
									throw std::runtime_error("Failed to insert item into item map!");
								}
							}
						} else {
							throw std::runtime_error("Failed to find value in kv pair!");
						}
					}
				}
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	void InventoryHandler::AddForm(const key_type& a_form, mapped_type a_count)
	{
		auto itemResult = _items.insert(std::make_pair(a_form, a_count));
		if (itemResult.first != _items.end()) {
			if (itemResult.second) {	// if new insertion
				auto formResult = _forms.insert({ itemResult.first->first->GetLoadedFormID(), FormList() });
				if (formResult.first != _forms.end()) {
					formResult.first->second.insert(a_form);	// push form into form list
				}
			} else {
				itemResult.first->second += a_count;	// else update count
			}

			if (itemResult.first->second <= 0) {	// if too few
				auto formIt = _forms.find(itemResult.first->first->GetLoadedFormID());
				if (formIt != _forms.end()) {
					auto formListIt = formIt->second.find(itemResult.first->first);	// locate form's entry in form list
					if (formListIt != formIt->second.end()) {
						formIt->second.erase(formListIt);	// delete entry in form list
					}
					if (formIt->second.empty()) {
						_forms.erase(formIt);	// delete form list if empty
					}
				}
				_items.erase(itemResult.first);	// delete entry in item map
			} else {
				// dispatch item add event
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
			// dispatch item remove event
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
