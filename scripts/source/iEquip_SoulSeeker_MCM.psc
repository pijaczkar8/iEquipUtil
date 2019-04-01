ScriptName iEquip_SoulSeeker_MCM Extends SKI_ConfigBase


Actor Property PlayerRef Auto
GlobalVariable Property SoulSeeker_FillMethod Auto  ; Float
GlobalVariable Property SoulSeeker_PartialFill Auto  ; Int
GlobalVariable Property SoulSeeker_WasteOK Auto  ; Int


Form[] g_forms
Int[] g_refHandles
Int g_arrSize


; Called when the config menu is initialized.
Event OnConfigInit()
	ModName = "Soul Seeker"
	pages = New String[1]
	pages[0] = "Config"

	g_forms = new Form[10]
	g_refHandles = new Int[10]
	g_arrSize = 0

	iEquip_InventoryExt.ParseInventory()
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
		AddTextOptionST("InventoryExt_RegisterForRefHandleActiveEvent_T", "Register for OnRefHandleActive event", "")
		AddTextOptionST("InventoryExt_RegisterForOnRefHandleInvalidatedEvent_T", "Register for OnRefHandleInvalidated event", "")
		AddTextOptionST("InventoryExt_DumpAll_T", "Dump info for all ref handles", "")
		AddTextOptionST("InventoryExt_GetRefHandleAtInvIndex_T", "GetRefHandleAtInvIndex", "")
		AddTextOptionST("InventoryExt_GetRefHandleFromWornObject_T", "GetRefHandleFromWornObject", "")
		AddTextOptionST("MyClass_HelloWorld_T", "Hello world", "")
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


State InventoryExt_RegisterForRefHandleActiveEvent_T
	Event OnSelectST()
		iEquip_InventoryExt.RegisterForRefHandleActiveEvent(Self)
		Debug.Trace("SoulSeekerDBG: Registered for OnRefHandleActive")
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State InventoryExt_RegisterForOnRefHandleInvalidatedEvent_T
	Event OnSelectST()
		iEquip_InventoryExt.RegisterForOnRefHandleInvalidatedEvent(Self)
		Debug.Trace("SoulSeekerDBG: Registered for OnRefHandleActive")
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State InventoryExt_DumpAll_T
	Event OnSelectST()
		Debug.Trace("")
		Int i = 0
		While (i < g_arrSize)
			String longName = iEquip_InventoryExt.GetLongName(g_forms[i], g_refHandles[i])
			String shortName = iEquip_InventoryExt.GetShortName(g_forms[i], g_refHandles[i])
			Int poisonCount = iEquip_InventoryExt.GetPoisonCount(g_forms[i], g_refHandles[i])
			Enchantment ench = iEquip_InventoryExt.GetEnchantment(g_forms[i], g_refHandles[i])
			Debug.Trace("SoulSeekerDBG: " + g_forms[i])
			Debug.Trace("Long Name == " + longName)
			Debug.Trace("Short Name == " + shortName)
			Debug.Trace("Poison Count == " + poisonCount)
			Debug.Trace("Enchantment == " + ench)
			i += 1
		EndWhile
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State InventoryExt_GetRefHandleAtInvIndex_T
	Event OnSelectST()
		Int handle

		handle = iEquip_InventoryExt.GetRefHandleAtInvIndex(0)
		Debug.Trace("SoulSeekerDBG: handle at idx 0 == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleAtInvIndex(1)
		Debug.Trace("SoulSeekerDBG: handle at idx 1 == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleAtInvIndex(2)
		Debug.Trace("SoulSeekerDBG: handle at idx 2 == " + handle)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State InventoryExt_GetRefHandleFromWornObject_T
	Event OnSelectST()
		Int handle

		handle = iEquip_InventoryExt.GetRefHandleFromWornObject(0)
		Debug.Trace("SoulSeekerDBG: handle from head == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleFromWornObject(1)
		Debug.Trace("SoulSeekerDBG: handle from chest == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleFromWornObject(2)
		Debug.Trace("SoulSeekerDBG: handle from boots == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleFromWornObject(3)
		Debug.Trace("SoulSeekerDBG: handle from gloves == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleFromWornObject(4)
		Debug.Trace("SoulSeekerDBG: handle from right hand == " + handle)

		handle = iEquip_InventoryExt.GetRefHandleFromWornObject(5)
		Debug.Trace("SoulSeekerDBG: handle from left hand == " + handle)
	EndEvent

	Event OnDefaultST()
	EndEvent

	Event OnHighlightST()
	EndEvent
EndState


State MyClass_HelloWorld_T
	Event OnSelectST()
	int[] arr
	int ARR_SIZE = 1
	arr = new int[1]
	int i = 0
	int j = ARR_SIZE - 1
	while (i < ARR_SIZE)
		arr[i] = j
		j -= 1
		i += 1
	EndWhile
	String result = MyClass.HelloWorld(arr)
	Debug.Trace("SoulSeekerDBG: " + result)
	i = 0
	while (i < ARR_SIZE)
		Debug.Trace(arr[i])
		i += 1
	EndWhile
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


Event OnBoundWeaponEquipped(Int a_weaponType, Int a_equipSlot)
	Debug.Trace("SoulSeekerDBG: OnBoundWeaponEquipped event recieved! (type == " + a_weaponType + ") (slot == " + a_equipSlot + ")")
EndEvent


Event OnBoundWeaponUnequipped(Weapon a_weap, Int a_unequipSlot)
	Debug.Trace("SoulSeekerDBG: OnBoundWeaponUnequipped event recieved! (name == " + a_weap.GetName() + ") (slot == " + a_unequipSlot + ")")
EndEvent


Event OnRefHandleActive(Form a_item, Int a_refHandle, Int a_itemCount)
	Debug.Trace("SoulSeekerDBG: OnRefHandleActive event recieved!")
	Debug.Trace("a_item == " + a_item)
	Debug.Trace("a_refHandle == " + a_refHandle)
	Debug.Trace("a_itemCount == " + a_itemCount)

	If (g_arrSize < 10)
		g_refHandles[g_arrSize] = a_refHandle
		g_forms[g_arrSize] = a_item
		g_arrSize += 1
	EndIf
EndEvent


Event OnRefHandleInvalidated(Form a_item, Int a_refHandle)
	Debug.Trace("SoulSeekerDBG: OnRefHandleInvalidated event recieved!")
	Debug.Trace("a_item == " + a_item)
	Debug.Trace("a_refHandle == " + a_refHandle)

	Int i = 0
	While (i < g_arrSize)
		If (g_refHandles[i] == a_refHandle)
			Int j = i + 1
			While (j < g_arrSize)
				g_refHandles[i] = g_refHandles[j]
				g_forms[i] = g_forms[j]
				i += 1
				j += 1
			EndWhile
			g_arrSize -= 1
			i = g_arrSize
		EndIf
		i += 1
	EndWhile
EndEvent
