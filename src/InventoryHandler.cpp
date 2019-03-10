#include "InventoryHandler.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges
#include "PluginAPI.h"  // SKSESerializationInterface

#include <string>  // to_string, stoi
#include <utility>  // make_pair

#include "ISerializableForm.h"  // ISerializableForm
#include "SerializableFormFactory.h"  // FormFactory

#include "RE/BSTList.h"  // BSSimpleList


namespace Forms
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
		return false;
	}


	bool InventoryHandler::Save(SKSESerializationInterface* a_intfc)
	{
		try {
			json formSave;
			json itemSave;
			for (auto& item : _items) {
				if (item.first->Save(formSave)) {
					itemSave = {
						{ FORMTYPE_STR, item.first->GetType() },
						{ COUNT_STR, item.second },
						{ FORM_STR, formSave }
					};
#if _DEBUG
					_DMESSAGE("\n%s", itemSave.dump(4).c_str());
#endif
					a_intfc->WriteRecord('INVH', SERIALIZATION_VERSION, itemSave.dump().c_str(), itemSave.dump().length() + 1);
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
#if _DEBUG
			_DMESSAGE("\n%s", a_load.dump(4).c_str());
#endif
			Forms::FormFactory* formFactory = Forms::FormFactory::GetSingleton();
			UInt32 formType;
			auto it = a_load.find(FORMTYPE_STR);
			if (it != a_load.end()) {
				formType = it.value();
			} else {
				throw std::runtime_error("Failed to find formtype in json load");
			}
			SerializableFormPtr form = formFactory->GetForm(formType, false);
			if (form) {
				UInt32 count;
				it = a_load.find(COUNT_STR);
				if (it != a_load.end()) {
					count = it.value();
				} else {
					throw std::runtime_error("Failed to find count in json load!");
				}

				it = a_load.find(FORM_STR);
				if (it != a_load.end()) {
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
					throw std::runtime_error("Failed to find form in json load!");
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
		if (itemResult.second) {
			auto formResult = _forms.insert({ itemResult.first->first->GetLoadedFormID(), FormList() });
			if (formResult.first != _forms.end()) {
				formResult.first->second.insert(a_form);	// push form into form list
			}
			// TODO: Dispatch item added event
		}
	}


	void InventoryHandler::UpdateCount(const key_type& a_form, mapped_type a_count)
	{
		auto itemIt = _items.find(a_form);
		if (itemIt != _items.end()) {
			itemIt->second += a_count;
			if (itemIt->second <= 0) {	// if too few
				auto formIt = _forms.find(a_form->GetLoadedFormID());
				if (formIt != _forms.end()) {
					auto formListIt = formIt->second.find(a_form);	// locate form's entry in form list
					if (formListIt != formIt->second.end()) {
						formIt->second.erase(formListIt);	// delete entry in form list
					}
					if (formIt->second.empty()) {
						_forms.erase(formIt);	// delete form list if empty
					}
				}
				_items.erase(itemIt);	// delete entry in item map
				// TODO: Dispatch item removed event
			}
		}
	}


	void InventoryHandler::RemoveFormListByID(UInt32 a_formID)
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
			// TODO: Dispatch item removed event
		}
	}


	void InventoryHandler::GarbageCollectoFormList(UInt32 a_formID)
	{
		auto formResult = _forms.find(a_formID);
		if (formResult == _forms.end()) {
			return;
		}

		InventoryEntryData* entryData = 0;
		::ExtraContainerChanges* changes = static_cast<::ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		if (changes && changes->data && changes->data->objList) {
			RE::BSSimpleList<InventoryEntryData*>* dataList = (RE::BSSimpleList<InventoryEntryData*>*)changes->data->objList;
			for (auto it = dataList->begin(); it != dataList->end(); ++it) {
				if ((*it)->type && (*it)->type->formID == a_formID) {
					entryData = *it;
					break;
				}
			}
		}

		if (entryData) {
			auto& formListRef = formResult->second;
			auto formListCopy = formResult->second;

			FormFactory* formFactory = FormFactory::GetSingleton();
			SerializableFormPtr form = nullptr;
			RE::BSSimpleList<::BaseExtraList*>* xList = (RE::BSSimpleList<::BaseExtraList*>*)entryData->extendDataList;
			if (xList) {
				for (auto it = xList->begin(); it != xList->end(); ++it) {
					form = formFactory->GetForm(entryData->type->formType, false);
					if (form) {
						form->Set(entryData->type, *it);
						formListCopy.erase(form);
					}
				}
			}

			form = formFactory->GetForm(entryData->type->formType, false);
			if (form) {
				formListCopy.erase(form);
			}

			for (auto& toRemove : formListCopy) {
				_items.erase(toRemove);
				formListRef.erase(toRemove);
				// TODO: Dispatch item removed event
			}
			if (formListRef.empty()) {
				_forms.erase(formResult);
			}
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
