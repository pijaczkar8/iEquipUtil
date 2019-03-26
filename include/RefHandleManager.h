#pragma once

#include "GameEvents.h"  // BSEventSink, EventResult, EventDispatcher

#include <limits>  // numeric_limits
#include <map>  // map
#include <set>  // set

#include "function_view.h"  // function_view

#include "RE/TESUniqueIDChangeEvent.h"  // TESUniqueIDChangeEvent

class BaseExtraList;
class BSExtraData;
class InventoryEntryData;
class TESForm;
struct SKSESerializationInterface;


class RefHandleManager : public BSTEventSink<RE::TESUniqueIDChangeEvent>
{
private:
	using SubType = UInt16;

public:
	using UniqueID = SubType;
	using RefHandle = SubType;
	using HandleResult = std::pair<RefHandle, bool>;


	struct EntryData
	{
		constexpr EntryData() : invEntryData(0), extraList(0) {}
		constexpr EntryData(InventoryEntryData* a_invEntryData, BaseExtraList* a_extraList) : invEntryData(a_invEntryData), extraList(a_extraList) {}


		InventoryEntryData* invEntryData;
		BaseExtraList* extraList;
	};


	static RefHandleManager* GetSingleton();

	void			Clear() noexcept;
	bool			Save(SKSESerializationInterface* a_intfc, UInt32 a_type, UInt32 a_version);
	bool			Load(SKSESerializationInterface* a_intfc, UInt32 a_version);
	HandleResult	ActivateHandle(TESForm* a_item, BaseExtraList*& a_extraList);
	HandleResult	ActivateHandle(TESForm* a_item, BaseExtraList& a_extraList);
	HandleResult	InvalidateHandle(TESForm* a_item, BaseExtraList* a_extraList);
	EntryData		LookupEntry(TESForm* a_form, RefHandle a_handle);
	bool			IsTrackedType(TESForm* a_form);

private:
	enum
	{
		kUniqueID = 0,
		kRefHandle,
		kTotal,

		kPlayerRefID = 0x14,

		kRefArrSize = std::numeric_limits<UInt16>::max() / 8,
		kLargestHandle = kRefArrSize * 8 - 1,
		kInvalidRefHandle = static_cast<RefHandle>(-1)
	};


	RefHandleManager();
	RefHandleManager(const RefHandleManager&) = delete;
	RefHandleManager(RefHandleManager&&) = delete;
	virtual ~RefHandleManager() = default;

	RefHandleManager& operator=(const RefHandleManager&) = delete;
	RefHandleManager& operator=(RefHandleManager&&) = delete;

	virtual	EventResult ReceiveEvent(RE::TESUniqueIDChangeEvent* a_event, EventDispatcher<RE::TESUniqueIDChangeEvent>* a_dispatcher) override;

	HandleResult	ActivateHandle(TESForm* a_item, BaseExtraList& a_extraList, RefHandle a_handle);
	RefHandle		GetFreeHandle();
	void			MarkHandle(RefHandle a_handle);
	void			UnmarkHandle(RefHandle a_handle);
	void			AddExtraData(BaseExtraList* a_extraList, BSExtraData* a_extraData);
	BaseExtraList*	CreateBaseExtraList();
	UniqueID		GetNextUniqueID();


	static const HandleResult		_NRES;
	std::map<UniqueID, RefHandle>	_idToHandleMap;
	std::map<RefHandle, UniqueID>	_handleToIDMap;
	UInt8							_activeHandles[kRefArrSize];
};
