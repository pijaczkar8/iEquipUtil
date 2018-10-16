ScriptName iEquip_FormExt


; @brief Registers the passed script to be notified when the player equips a bound weapon.
; @param a_thisForm The form to register for the event (i.e. Self).
Function RegisterForBoundWeaponEquippedEvent(Form a_thisForm) Global Native


; @brief Fires whenever the player equips a bound weapon.
; @param a_weaponType The type of weapon equipped.
; VALID TYPES:
; 00 - Hand-to-hand melee
; 01 - One-handed sword
; 02 - One-handed dagger
; 03 - One-handed axe
; 04 - One-handed mace
; 05 - Two-handed sword
; 06 - Two-handed axe
; 07 - Bow
; 08 - Staff
; 09 - Crossbow
; 10 - Hand-to-hand melee
; 11 - One-handed sword
; 12 - One-handed dagger
; 13 - One-handed axe
; 14 - One-handed mace
; 15 - Two-handed sword
; 16 - Two-handed axe
; 17 - Bow
; 18 - Staff
; 19 - Crossbow
Event OnBoundWeaponEquipped(Int a_weaponType)
EndEvent