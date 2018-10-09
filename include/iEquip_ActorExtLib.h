#pragma once

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // AlchemyItem, EnchantmentItem


namespace iEquip_ActorExt
{
	enum
	{
		kSlotId_Default = 0,
		kSlotId_Right = 1,
		kSlotId_Left = 2
	};


	InventoryEntryData* findEntryData(ExtraContainerChanges::Data* a_containerData, TESForm* a_item);
	BGSEquipSlot* getEquipSlotByID(SInt32 a_slotID);
	bool CanEquipBothHands(Actor* a_actor, TESForm* a_item);


	class IActorEquipItem
	{
	public:
		IActorEquipItem() {}
		virtual ~IActorEquipItem() {}

		virtual bool validate() = 0;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) = 0;
	};


	class ActorEquipPoisonedItem : virtual public IActorEquipItem
	{
	public:
		explicit ActorEquipPoisonedItem(AlchemyItem* a_poison);
		virtual ~ActorEquipPoisonedItem();

		virtual bool validate() override;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) override;

	protected:
		AlchemyItem* _poison;
	};


	class ActorEquipEnchantedItem : virtual public IActorEquipItem
	{
	public:
		explicit ActorEquipEnchantedItem(EnchantmentItem* a_enchantment);
		virtual ~ActorEquipEnchantedItem();

		virtual bool validate() override;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) override;

	protected:
		EnchantmentItem* _enchantment;
	};


	class ActorEquipPoisonedAndEnchantedItem : public ActorEquipPoisonedItem, public ActorEquipEnchantedItem
	{
	public:
		explicit ActorEquipPoisonedAndEnchantedItem(AlchemyItem* a_poison, EnchantmentItem* a_enchantment);
		virtual ~ActorEquipPoisonedAndEnchantedItem();

		virtual bool validate() override;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) override;
	};
}