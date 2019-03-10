#pragma once

class SpellItem;
class VMClassRegistry;
struct StaticFunctionTag;


enum ActorValue : UInt32
{
	kActorValue_Conjuration = 19,
	kActorValue_Restoration = 22,
	kActorValue_Health = 24
};


bool	IsHealingSpell(StaticFunctionTag* a_base, SpellItem* a_spell);
bool	IsBoundSpell(StaticFunctionTag* a_base, SpellItem* a_spell);
SInt32	GetBoundSpellWeapType(StaticFunctionTag* a_base, SpellItem* a_spell);


namespace SpellExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
