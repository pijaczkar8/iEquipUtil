#pragma once

class SpellItem;
class VMClassRegistry;
struct StaticFunctionTag;


namespace SpellExt
{
	namespace
	{
		enum ActorValue : UInt32
		{
			kActorValue_Conjuration = 19,
			kActorValue_Restoration = 22,
			kActorValue_Health = 24
		};
	}


	bool	IsHealingSpell(StaticFunctionTag*, SpellItem* a_spell);
	bool	IsBoundSpell(StaticFunctionTag*, SpellItem* a_spell);
	SInt32	GetBoundSpellWeapType(StaticFunctionTag*, SpellItem* a_spell);
	bool	RegisterFuncs(VMClassRegistry* a_registry);
}
