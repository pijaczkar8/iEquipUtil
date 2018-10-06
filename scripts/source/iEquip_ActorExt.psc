ScriptName iEquip_ActorExt


; @brief Force equips the poisoned item with the specified FormID to the specified slot.
; @param a_actor The actor to equip the item to.
; @param a_item The item to equip.
; @param a_slotID The slot ID to equip the item to.
; @param a_extraForm The Form of the extra data applied to the weapon.
; @param a_preventUnequip Prevents the actor from unequipping the item.
; @param a_equipSound Plays the equip sound when the item is equipped.
Function EquipItemEx(Actor a_actor, Form a_item, Int a_slotID, Form a_extraForm, Bool a_preventUnequip = False, Bool a_equipSound = True) Native Global