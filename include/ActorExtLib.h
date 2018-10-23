#pragma once

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // AlchemyItem, EnchantmentItem
#include "ITypes.h"  // UInt32


namespace iEquip
{
	enum
	{
		kSlotID_Default = 0,
		kSlotID_Right = 1,
		kSlotID_Left = 2
	};


	InventoryEntryData* findEntryData(ExtraContainerChanges::Data* a_containerData, TESForm* a_item);
	BGSEquipSlot* getEquipSlotByID(SInt32 a_slotID);
	bool canEquipBothHands(Actor* a_actor, TESForm* a_item);

	/* @brief Checks in which slots the given weapon is equipped by the given actor.
	 * @param a_actor The actor to check.
	 * @param a_weap The weapon to check for.
	 * @return Returns the slot the weapon is equipped in.
	 * RETURN TYPES:
	 * 0 - Unequipped
	 * 1 - Right hand
	 * 2 - Left hand
	 * 3 - Dual wield
	 */
	UInt32 getEquippedSlots(Actor* a_actor, TESObjectWEAP* a_weap);

	/* @brief Checks which slots the given actor does not have a weapon equipped in.
	 * @param a_actor The actor to check.
	 * @return Returns the slot the weapons are equipped to.
	 * RETURN TYPES:
	 * 0 - Fists
	 * 1 - Right hand
	 * 2 - Left hand
	 * 3 - Dual wield
	 */
	UInt32 getUnequippedSlots(Actor* a_actor);


	class IActorEquipItem
	{
	public:
		IActorEquipItem() {}
		virtual ~IActorEquipItem() {}

		virtual bool validate() = 0;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) = 0;
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


	class ActorEquipPoisonedItem : virtual public IActorEquipItem
	{
	public:
		explicit ActorEquipPoisonedItem(AlchemyItem* a_poison, UInt32 a_count);
		virtual ~ActorEquipPoisonedItem();

		virtual bool validate() override;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) override;

	protected:
		AlchemyItem* _poison;
		UInt32 _count;
	};


	class ActorEquipEnchantedAndPoisonedItem : public ActorEquipEnchantedItem, public ActorEquipPoisonedItem
	{
	public:
		explicit ActorEquipEnchantedAndPoisonedItem(EnchantmentItem* a_enchantment, AlchemyItem* a_poison, UInt32 a_count);
		virtual ~ActorEquipEnchantedAndPoisonedItem();

		virtual bool validate() override;
		virtual BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData) override;
	};
}