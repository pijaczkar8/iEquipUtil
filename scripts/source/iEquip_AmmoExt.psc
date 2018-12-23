ScriptName iEquip_AmmoExt


; @brief Returns whether the given ammo is bound or not.
; @param a_ammo The ammo to check.
; @return Returns true if the ammo is bound, else returns false.
Bool Function IsAmmoBound(Ammo a_ammo) Global Native


; @brief Returns whether the given ammo is a javelin or not.
; @param a_ammo The ammo to check.
; @return Returns true if the ammo is a javelin, else returns false.
Bool Function IsAmmoJavelin(Ammo a_ammo) Global Native


; @brief Returns the given ammo's element
; @param a_ammo The ammo to check
; @return Returns the element of the given ammo
; VALID RETURN VALUES:
; 0 - None
; 1 - Telekenesis
; 2 - SoulStealer
; 3 - Fire
; 4 - Ice
; 5 - Lightning
; 6 - Bound
; 7 - Bone
Int Function GetAmmoElement(Ammo a_ammo) Global Native
