ScriptName iEquip_ActorExt


; @brief Force equips the poisoned item with the specified FormID to the specified slot.
; @param a_actor The actor to equip the item to.
; @param a_item The item to equip.
; @param a_itemID The FormID of the item to equip.
; @param a_slotID The slot ID to equip the item to.
; @param a_preventUnequip Prevents the actor from unequipping the item (I don't think this does anything).
; @param a_equipSound Plays the equip sound when the item is equipped.
; @param a_poison The poison applied to the weapon.
Function EquipPoisonedItemByID(Actor a_actor, Form a_item, Int a_itemID, Int a_slotID, Bool a_preventUnequip, Bool a_equipSound, Form a_poison) Native Global