#pragma once

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // AlchemyItem, EnchantmentItem


namespace iEquip_ActorExt
{
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