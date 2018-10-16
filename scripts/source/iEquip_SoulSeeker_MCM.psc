ScriptName iEquip_SoulSeeker_MCM Extends SKI_ConfigBase


Actor Property PlayerRef Auto
GlobalVariable Property SoulSeeker_FillMethod Auto  ; Float
GlobalVariable Property SoulSeeker_PartialFill Auto  ; Int
GlobalVariable Property SoulSeeker_WasteOK Auto  ; Int


; Called when the config menu is initialized.
Event OnConfigInit()
	ModName = "Soul Seeker"
	pages = New String[1]
	pages[0] = "Config"
EndEvent


; Called when the config menu is closed.
Event OnConfigClose()
EndEvent


; Called when a version update of this script has been detected.
; a_version - The new version.
Event OnVersionUpdate(Int a_version)
EndEvent


; Called when a new page is selected, including the initial empty page.
; a_page - The name of the the current page, or "" if no page is selected.
Event OnPageReset(String a_page)
	If (a_page == "Config")
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddTextOptionST("SoulSeeker_Petty_T", "Remove Petty", "")
		AddTextOptionST("SoulSeeker_Lesser_T", "Remove Lesser", "")
		AddTextOptionST("SoulSeeker_Common_T", "Remove Common", "")
		AddTextOptionST("SoulSeeker_Greater_T", "Remove Greater", "")
		AddTextOptionST("SoulSeeker_Grand_T", "Remove Grand", "")
		AddHeaderOption("")
		AddTextOptionST("ActorExt_EquipEnch_T", "Equip enchanted item", "")
		AddTextOptionST("ActorExt_EquipPoisoned_T", "Equip poisoned item", "")
		AddTextOptionST("ActorExt_EquipEnchAndPoisoned_T", "Equip enchanted and poisoned item", "")
		AddTextOptionST("ActorExt_GetEquipped_T", "Get equipped ammo form", "")
		AddTextOptionST("WeaponExt_IsWeaponBound_T", "Check if right hand is bound", "")
		AddTextOptionST("WeaponExt_IsAmmoBound_T", "Check if equipped ammo is bound", "")
		AddTextOptionST("FormExt_RegisterBoundWeaponEquipped_T", "Register for OnBoundWeaponEquipped event", "")
		SetCursorPosition(1)
		AddSliderOptionST("SoulSeeker_FillMethod_S", "Fill Method:", SoulSeeker_FillMethod.GetValue() As Float)
		AddToggleOptionST("SoulSeeker_PartialFill_B", "Partial Fill:", SoulSeeker_PartialFill.GetValue() As Bool)
		AddToggleOptionST("SoulSeeker_WasteOK_B", "Waste OK:", SoulSeeker_WasteOK.GetValue() As Bool)
	EndIf
EndEvent


State SoulSeeker_Petty_T
	Event OnSelectST()
		InvokeBringMeASoul(1)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_Lesser_T
	Event OnSelectST()
		InvokeBringMeASoul(2)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_Common_T
	Event OnSelectST()
		InvokeBringMeASoul(3)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_Greater_T
	Event OnSelectST()
		InvokeBringMeASoul(4)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_Grand_T
	Event OnSelectST()
		InvokeBringMeASoul(5)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State ActorExt_EquipEnch_T
	Event OnSelectST()
		Form steelSword = Game.GetForm(0x00013989)
		Enchantment ench = WornObject.GetEnchantment(PlayerRef, 0, 0)
		iEquip_ActorExt.EquipEnchantedItemEx(PlayerRef, steelSword, 1, ench)
		Debug.Trace("SoulSeekerDBG: EquipEnchantedItemEx called!")
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State ActorExt_EquipPoisoned_T
	Event OnSelectST()
		Form steelSword = Game.GetForm(0x00013989)
		Potion weakPoison = Game.GetForm(0x0003A5A4) As Potion
		iEquip_ActorExt.EquipPoisonedItemEx(PlayerRef, steelSword, 1, weakPoison)
		Debug.Trace("SoulSeekerDBG: EquipPoisonedItemEx called!")
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State ActorExt_EquipEnchAndPoisoned_T
	Event OnSelectST()
		Form steelSword = Game.GetForm(0x00013989)
		Potion weakPoison = Game.GetForm(0x0003A5A4) As Potion
		Enchantment ench = WornObject.GetEnchantment(PlayerRef, 0, 0)
		iEquip_ActorExt.EquipEnchantedAndPoisonedItemEx(PlayerRef, steelSword, 1, ench, weakPoison)
		Debug.Trace("SoulSeekerDBG: EquipEnchantedAndPoisonedItemEx called!")
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State ActorExt_GetEquipped_T
	Event OnSelectST()
		Form myForm = NONE
		myForm = iEquip_ActorExt.GetEquippedAmmo(PlayerRef)
		If (myForm)
			Debug.Trace("SoulSeekerDBG: GetEquippedAmmo() success!")
		Else
			Debug.Trace("SoulSeekerDBG: GetEquippedAmmo() failed!")
		EndIf
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State WeaponExt_IsWeaponBound_T
	Event OnSelectST()
		Weapon weap = PlayerRef.GetEquippedWeapon()
		If (iEquip_WeaponExt.IsWeaponBound(weap))
			Debug.Trace("SoulSeekerDBG: Player's right hand weapon is bound!")
		Else
			Debug.Trace("SoulSeekerDBG: Player's right hand weapon is not bound!")
		EndIf
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State WeaponExt_IsAmmoBound_T
	Event OnSelectST()
		Ammo myAmmo = iEquip_ActorExt.GetEquippedAmmo(PlayerRef)
		If (iEquip_AmmoExt.IsAmmoBound(myAmmo))
			Debug.Trace("SoulSeekerDBG: Player's equipped ammo is bound!")
		Else
			Debug.Trace("SoulSeekerDBG: Player's equipped ammo is not bound!")
		EndIf
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State FormExt_RegisterBoundWeaponEquipped_T
	Event OnSelectST()
		iEquip_FormExt.RegisterForBoundWeaponEquippedEvent(Self)
		Debug.Trace("SoulSeekerDBG: Registered for OnBoundWeaponEquipped")
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_FillMethod_S
	Event OnSliderOpenST()
		SetSliderDialogStartValue(SoulSeeker_FillMethod.GetValue() As Float)
		SetSliderDialogDefaultValue(1)
		SetSliderDialogRange(0, 1)
		SetSliderDialogInterval(1)
	EndEvent

	Event OnSliderAcceptST(Float a_value)
		SoulSeeker_FillMethod.SetValue(a_value)
		SetSliderOptionValueST(SoulSeeker_FillMethod.GetValue() As Float)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_PartialFill_B
	Event OnSelectST()
		If (SoulSeeker_PartialFill.GetValue() As Bool)
			SoulSeeker_PartialFill.SetValue(0)
		Else
			SoulSeeker_PartialFill.SetValue(1)
		EndIf
		SetToggleOptionValueST(SoulSeeker_PartialFill.GetValue() As Int)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State SoulSeeker_WasteOK_B
	Event OnSelectST()
		If (SoulSeeker_WasteOK.GetValue() As Bool)
			SoulSeeker_WasteOK.SetValue(0)
		Else
			SoulSeeker_WasteOK.SetValue(1)
		EndIf
		SetToggleOptionValueST(SoulSeeker_WasteOK.GetValue() As Int)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


; Returns the static version of this script.
; RETURN - The static version of this script.
; History:
; 1 - Initial Release (v1.0.0)
Int Function GetVersion()
	Return 2
EndFunction


Function InvokeBringMeASoul(Int a_reqCharge)
	Int fillMethod = SoulSeeker_FillMethod.GetValue() As Int
	Bool partialFill = SoulSeeker_PartialFill.GetValue() As Bool
	Bool wasteOK = SoulSeeker_WasteOK.GetValue() As Bool
	Int soulSize = iEquip_SoulSeeker.BringMeASoul(a_reqCharge, fillMethod, partialFill, wasteOK)
	Debug.Trace("SoulSeekerDBG: BringMeASoul() returned soulsize == " + soulSize)
EndFunction


Event OnBoundWeaponEquipped(Int a_weaponType)
	Debug.Trace("SoulSeekerDBG: OnBoundWeaponEquipped event recieved! (weaponType == " + a_weaponType + ")")
EndEvent