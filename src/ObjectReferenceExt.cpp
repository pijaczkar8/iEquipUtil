#include "ObjectReferenceExt.h"

#include "GameExtraData.h"  // ExtraTextDisplayData
#include "GameFormComponents.h"  // TESFullName
#include "GameReferences.h"  // TESObjectREFR
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "GameTypes.h"  // BSFixedString
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <string>  // string

#include "RE/ExtraTextDisplayData.h"  // ExtraTextDisplayData



BSFixedString GetDisplayName(StaticFunctionTag* a_base, TESObjectREFR* a_obj)
{
	if (!a_obj) {
		_ERROR("[ERROR] a_obj is a NONE form!\n");
		return "";
	}

	RE::ExtraTextDisplayData* xText = static_cast<RE::ExtraTextDisplayData*>(a_obj->extraData.GetByType(kExtraData_TextDisplayData));
	if (xText) {
		std::string name(xText->name.data, xText->rawNameLen);
		return name.c_str();
	} else {
		TESFullName* name = DYNAMIC_CAST(a_obj->baseForm, TESForm, TESFullName);
		return name ? name->GetName() : "";
	}
}


namespace ObjectReferenceExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, BSFixedString, TESObjectREFR*>("GetDisplayName", "iEquip_ObjectReferenceExt", GetDisplayName, a_registry));

		return true;
	}
}
