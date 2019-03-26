#include "RefHandleManager.h"

#include "GameAPI.h"  // g_thePlayer
#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, ExtraUniqueID
#include "GameForms.h"  // TESForm
#include "GameReferences.h"  // PlayerCharacter
#include "PluginAPI.h"  // SKSESerializationInterface

#include <algorithm>  // sort
#include <cstring>  // memset
#include <type_traits>  // remove_extent_t, underlying_type_t
#include <vector>  // vector

#include "InventoryUtil.h"  // ForEachInvEntry, ForEachExtraList

#include "RE/BSTList.h"  // BSSimpleList


RefHandleManager* RefHandleManager::GetSingleton()
{
	static RefHandleManager singleton;
	return &singleton;
}


void RefHandleManager::Clear() noexcept
{
	_idToHandleMap.clear();
	_handleToIDMap.clear();
	std::memset(_activeHandles, 0, sizeof(_activeHandles));
}


bool RefHandleManager::Save(SKSESerializationInterface* a_intfc, UInt32 a_type, UInt32 a_version)
{
	a_intfc->OpenRecord(a_type, a_version);
	std::size_t numEntries = _idToHandleMap.size();
	a_intfc->WriteRecordData(&numEntries, sizeof(numEntries));

	SubType buf[kTotal];
	for (auto& it : _idToHandleMap) {
		buf[kUniqueID] = it.first;
		buf[kRefHandle] = it.second;
		a_intfc->WriteRecordData(&buf, sizeof(buf));
	}

	return true;
}


bool RefHandleManager::Load(SKSESerializationInterface* a_intfc, UInt32 a_version)
{
	std::size_t numEntries;
	if (!a_intfc->ReadRecordData(&numEntries, sizeof(numEntries))) {
		_ERROR("[ERROR] Failed to load handle count!\n");
		return false;
	}

	SubType buf[kTotal];
	for (std::size_t i = 0; i < numEntries; ++i) {
		if (a_intfc->ReadRecordData(&buf, sizeof(buf))) {
			_idToHandleMap.insert({ buf[kUniqueID], buf[kRefHandle] });
			_handleToIDMap.insert({ buf[kRefHandle], buf[kUniqueID] });
			MarkHandle(buf[kRefHandle]);
		} else {
			_ERROR("[ERROR] Failed to load ref handles!\n");
			return false;
		}
	}

	return true;
}


auto RefHandleManager::ActivateHandle(TESForm* a_item, BaseExtraList*& a_extraList)
-> HandleResult
{
	if (!IsTrackedType(a_item)) {
		return _NRES;
	}

	RefHandle handle = GetFreeHandle();
	if (handle == kInvalidRefHandle) {
		return _NRES;
	}

	if (!a_extraList) {
		a_extraList = CreateBaseExtraList();
	}

	return ActivateHandle(a_item, *a_extraList, handle);
}


auto RefHandleManager::ActivateHandle(TESForm* a_item, BaseExtraList& a_extraList)
-> HandleResult
{
	if (!IsTrackedType(a_item)) {
		return _NRES;
	}

	RefHandle handle = GetFreeHandle();
	if (handle == kInvalidRefHandle) {
		return _NRES;
	}

	return ActivateHandle(a_item, a_extraList, handle);
}


auto RefHandleManager::InvalidateHandle(TESForm* a_item, BaseExtraList* a_extraList)
-> HandleResult
{
	if (!IsTrackedType(a_item)) {
		return _NRES;
	}

	auto xID = a_extraList ? static_cast<ExtraUniqueID*>(a_extraList->GetByType(kExtraData_UniqueID)) : 0;
	if (!xID) {
		return _NRES;
	}

	auto it = _idToHandleMap.find(xID->uniqueId);
	RefHandle handle = it->second;
	_handleToIDMap.erase(handle);
	_idToHandleMap.erase(it);
	UnmarkHandle(handle);

	return { handle , true };
}


auto RefHandleManager::LookupEntry(TESForm* a_form, RefHandle a_handle)
-> EntryData
{
	EntryData entryData;
	if (a_handle > kLargestHandle) {
		_ERROR("[ERROR] Ref handle is invalid!\n");
		return entryData;
	}

	auto it = _handleToIDMap.find(a_handle);
	if (it == _handleToIDMap.end()) {
		return entryData;
	}

	ForEachInvEntry([&](InventoryEntryData* a_invEntryData) -> bool
	{
		if (a_invEntryData->type->formID == a_form->formID) {
			ForEachExtraList(a_invEntryData, [&](BaseExtraList* a_extraList) -> bool
			{
				auto xID = static_cast<ExtraUniqueID*>(a_extraList->GetByType(kExtraData_UniqueID));
				if (xID && xID->uniqueId == it->second) {
					entryData.extraList = a_extraList;
					entryData.invEntryData = a_invEntryData;
					return false;
				} else {
					return true;
				}
			});
			return false;
		} else {
			return true;
		}
	});

	return entryData;
}


bool RefHandleManager::IsTrackedType(TESForm* a_form)
{
	switch (a_form->formType) {
	case kFormType_Armor:
	case kFormType_Weapon:
		return true;
	default:
		return false;
	}
}


RefHandleManager::RefHandleManager() :
	_activeHandles{ 0 }
{}


EventResult RefHandleManager::ReceiveEvent(RE::TESUniqueIDChangeEvent* a_event, EventDispatcher<RE::TESUniqueIDChangeEvent>* a_dispatcher)
{
	if (a_event->ownerFormID != kPlayerRefID || a_event->oldUniqueID == a_event->newUniqueID) {
		return kEvent_Continue;
	}

	auto it = _idToHandleMap.find(a_event->oldUniqueID);
	if (it != _idToHandleMap.end()) {
		RefHandle handle = it->second;
		_handleToIDMap.erase(handle);
		_idToHandleMap.erase(it);

		_handleToIDMap.insert({ handle, a_event->newUniqueID });
		_idToHandleMap.insert({ a_event->newUniqueID, handle });
	}

	return kEvent_Continue;
}


auto RefHandleManager::ActivateHandle(TESForm* a_item, BaseExtraList& a_extraList, RefHandle a_handle)
-> HandleResult
{
	auto xID = static_cast<ExtraUniqueID*>(a_extraList.GetByType(kExtraData_UniqueID));
	if (xID) {
		_ERROR("[ERROR] Item already has ExtraUniqueID!\n");
		UnmarkHandle(a_handle);
		return _NRES;
	} else {
		xID = ExtraUniqueID::Create();
		xID->ownerFormId = kPlayerRefID;
		xID->uniqueId = GetNextUniqueID();
		AddExtraData(&a_extraList, xID);
	}

	_idToHandleMap.insert({ xID->uniqueId, a_handle });
	_handleToIDMap.insert({ a_handle, xID->uniqueId });
	return { a_handle, true };
}


auto RefHandleManager::GetFreeHandle()
-> RefHandle
{
	for (std::size_t i = 0; i < kRefArrSize; ++i) {
		if (_activeHandles[i] != 0xFF) {
			for (UInt8 j = 0; j < 8; ++j) {
				if (((_activeHandles[i] >> j) & 1) == 0) {
					_activeHandles[i] |= 1 << j;
					return (i * 8) + j;
				}
			}
		}
	}
	_FATALERROR("[FATAL ERROR] Ran out of ref handles!\n");
	return kInvalidRefHandle;
}


void RefHandleManager::MarkHandle(RefHandle a_handle)
{
	constexpr std::size_t SIZE = sizeof(std::remove_extent_t<decltype(_activeHandles)>) * 8;
	std::size_t idx = a_handle / SIZE;
	UInt8 bit = 1 << (a_handle % SIZE);
	_activeHandles[idx] |= bit;
}


void RefHandleManager::UnmarkHandle(RefHandle a_handle)
{
	constexpr std::size_t SIZE = sizeof(std::remove_extent_t<decltype(_activeHandles)>) * 8;
	std::size_t idx = a_handle / SIZE;
	UInt8 bit = 1 << (a_handle % SIZE);
	_activeHandles[idx] &= ~bit;
}


void RefHandleManager::AddExtraData(BaseExtraList* a_extraList, BSExtraData* a_extraData)
{
	using func_t = BSExtraData * (BaseExtraList*, BSExtraData*);
#if _WIN64
	// E8 ? ? ? ? 4C 8D 75 10
	RelocAddr<func_t*> func(0x00131990);	// 1_5_73
#else
	func_t* func = reinterpret_cast<func_t*>(0x0040A790);
#endif
	func(a_extraList, a_extraData);
}


BaseExtraList* RefHandleManager::CreateBaseExtraList()
{
	constexpr std::size_t XLIST_SIZE = sizeof(BaseExtraList);

	auto xList = static_cast<BaseExtraList*>(Heap_Allocate(XLIST_SIZE));
	std::memset(xList, 0, XLIST_SIZE);

	return xList;
}


auto RefHandleManager::GetNextUniqueID()
-> UniqueID
{
	using func_t = UniqueID(ExtraContainerChanges::Data*);
#if _WIN64
	// E8 ? ? ? ? 44 0F B7 F8
	RelocAddr<func_t*> func(0x001ECD30);	// 1_5_73
#else
	auto func = reinterpret_cast<func_t*>(0x00481EF0);
#endif
	auto containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
	return func(containerChanges->data);
}


decltype(RefHandleManager::_NRES) RefHandleManager::_NRES{ kInvalidRefHandle, false };
