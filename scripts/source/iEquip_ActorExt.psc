ScriptName iEquip_ActorExt


Bool Function IsEquipped() Global Native


; @brief Checks if the given weapon is a bound weapon or not.
; @param a_weap The weapon to check.
; @return Returns true if the weapon is bound. Returns false if the weapon is invalid or not bound.
Bool Function IsWeaponBound(Weapon a_weap) Native Global


; @brief Force equips the item with the specified FormID and applied poison to the specified slot.
; @param a_actor The actor to equip the item to.
; @param a_item The item to equip.
; @param a_slotID The slot ID to equip the item to.
; @param a_poison The form of the poison applied to the item.
; @param a_preventUnequip Prevents the actor from unequipping the item.
; @param a_equipSound Plays the equip sound when the item is equipped.
Function EquipPoisonedItemEx(Actor a_actor, Form a_item, Int a_slotID, Potion a_poison, Bool a_preventUnequip = False, Bool a_equipSound = True) Native Global


; @brief Force equips the item with the specified FormID and applied poison to the specified slot.
; @param a_actor The actor to equip the item to.
; @param a_item The item to equip.
; @param a_slotID The slot ID to equip the item to.
; @param a_enchantment The form of the enchantment applied to the item.
; @param a_preventUnequip Prevents the actor from unequipping the item.
; @param a_equipSound Plays the equip sound when the item is equipped.
; @notes a_enchantment must be a direct reference to the enchantment applied to the weapon. You must call GetEnchantment() on the weapon and pass in the return value. I'll look into reworking this.
Function EquipEnchantedItemEx(Actor a_actor, Form a_item, Int a_slotID, Enchantment a_enchantment, Bool a_preventUnequip = False, Bool a_equipSound = True) Native Global


; @brief Force equips the item with the specified FormID, poison, and enchantment to the specified slot.
; @param a_actor The actor to equip the item to.
; @param a_item The item to equip.
; @param a_slotID The slot ID to equip the item to.
; @param a_poison The Form of the poison applied to the weapon.
; @param a_enchantment The Form of the enchantment applied to the weapon.
; @param a_preventUnequip Prevents the actor from unequipping the item.
; @param a_equipSound Plays the equip sound when the item is equipped.
; @notes Same usage notes apply from EquipEnchantedItemEx().
Function EquipPoisonedAndEnchantedItemEx(Actor a_actor, Form a_item, Int a_slotID, Potion a_poison, Enchantment a_enchantment, Bool a_preventUnequip = False, Bool a_equipSound = True) Native Global