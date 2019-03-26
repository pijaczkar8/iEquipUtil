#pragma once

#include "GameEvents.h"  // EventDispatcher, EventResult
#include "GameObjects.h"  // TESObjectWEAP
#include "GameReferences.h"  // Actor

#include "RE/TESEquipEvent.h"  // TESEquipEvent
#include "RE/TESUniqueIDChangeEvent.h"  // TESUniqueIDChangeEvent


namespace RE
{
	struct BGSEventProcessedEvent;
	struct TESActivateEvent;
	struct TESActorLocationChangeEvent;
	struct TESBookReadEvent;
	struct TESCellFullyLoadedEvent;
	struct TESCellReadyToApplyDecalsEvent;
	struct TESDestructionStageChangedEvent;
	struct TESEnterBleedoutEvent;
	struct TESEquipEvent;
	struct TESFormDeleteEvent;
	struct TESFurnitureEvent;
	struct TESGrabReleaseEvent;
	struct TESHitEvent;
	struct TESLoadGameEvent;
	struct TESLockChangedEvent;
	struct TESMagicEffectApplyEvent;
	struct TESMagicWardHitEvent;
	struct TESMoveAttachDetachEvent;
	struct TESObjectLoadedEvent;
	struct TESObjectREFRTranslationEvent;
	struct TESOpenCloseEvent;
	struct TESPackageEvent;
	struct TESPerkEntryRunEvent;
	struct TESQuestInitEvent;
	struct TESQuestStageEvent;
	struct TESQuestStageItemDoneEvent;
	struct TESQuestStartStopEvent;
	struct TESResetEvent;
	struct TESResolveNPCTemplatesEvent;
	struct TESSceneEvent;
	struct TESSceneActionEvent;
	struct TESScenePhaseEvent;
	struct TESSellEvent;
	struct TESSleepStartEvent;
	struct TESSleepStopEvent;
	struct TESSpellCastEvent;
	struct TESPlayerBowShotEvent;
	struct TESTopicInfoEvent;
	struct TESTrackedStatsEvent;
	struct TESTrapHitEvent;
	struct TESTriggerEvent;
	struct TESTriggerEnterEvent;
	struct TESTriggerLeaveEvent;


#if _WIN64
	class EventDispatcherList
	{
	public:
		EventDispatcher<BGSEventProcessedEvent>				eventProcessedEventSource;			// 0000 - sink offset 000
		EventDispatcher<TESActivateEvent>					activateEventSource;				// 0058 - sink offset 008
		EventDispatcher<TESActiveEffectApplyRemoveEvent>	activeEffectApplyRemoveEventSource;	// 00B0 - sink offset 010
		EventDispatcher<TESActorLocationChangeEvent>		actorLocationChangeEventSource;		// 0108 - sink offset 018
		EventDispatcher<TESBookReadEvent>					bookReadEventSource;				// 0160 - sink offset 020
		EventDispatcher<TESCellAttachDetachEvent>			cellAttachDetachEventSource;		// 01B8 - sink offset 028
		EventDispatcher<TESCellFullyLoadedEvent>			cellFullyLoadedEventSource;			// 0210 - sink offset 030
		EventDispatcher<TESCellReadyToApplyDecalsEvent>		cellReadyToApplyDecalsEventSource;	// 0268 - sink offset 038
		EventDispatcher<TESCombatEvent>						combatEventSource;					// 02C0 - sink offset 040
		EventDispatcher<TESContainerChangedEvent>			containerChangedEventSource;		// 0318 - sink offset 048
		EventDispatcher<TESDeathEvent>						deathEventSource;					// 0370 - sink offset 050
		EventDispatcher<TESDestructionStageChangedEvent>	destructionStageChangedEventSource;	// 03C8 - sink offset 058
		EventDispatcher<TESEnterBleedoutEvent>				enterBleedoutEventSource;			// 0420 - sink offset 060
		EventDispatcher<TESEquipEvent>						equipEventSource;					// 0478 - sink offset 078
		EventDispatcher<TESFormDeleteEvent>					formDeleteEventSource;				// 04D0 - sink offset 080
		EventDispatcher<TESFurnitureEvent>					furnitureEventSource;				// 0528 - sink offset 088
		EventDispatcher<TESGrabReleaseEvent>				grabReleaseEventSource;				// 0580 - sink offset 090
		EventDispatcher<TESHitEvent>						hitEventSource;						// 05D8 - sink offset 098
		EventDispatcher<TESInitScriptEvent>					initScriptEventSource;				// 0630 - sink offset 0A0
		EventDispatcher<TESLoadGameEvent>					loadGameEventSource;				// 0688 - sink offset 0A8
		EventDispatcher<TESLockChangedEvent>				lockChangedEventSource;				// 06E0 - sink offset 0B0
		EventDispatcher<TESMagicEffectApplyEvent>			magicEffectApplyEventSource;		// 0738 - sink offset 0B8
		EventDispatcher<TESMagicWardHitEvent>				magicWardHitEventSource;			// 0790 - sink offset 0C0
		EventDispatcher<TESMoveAttachDetachEvent>			moveAttachDetachEventSource;		// 07E8 - sink offset 0C8
		EventDispatcher<TESObjectLoadedEvent>				objectLoadedEventSource;			// 0840 - sink offset 0D0
		EventDispatcher<TESObjectREFRTranslationEvent>		objectREFRTranslationEventSource;	// 0898 - sink offset 0D8
		EventDispatcher<TESOpenCloseEvent>					openCloseEventSource;				// 08F0 - sink offset 0E0
		EventDispatcher<TESPackageEvent>					packageEventSource;					// 0948 - sink offset 0E8
		EventDispatcher<TESPerkEntryRunEvent>				perkEntryRunEventSource;			// 09A0 - sink offset 0F0
		EventDispatcher<TESQuestInitEvent>					questInitEventSource;				// 09F8 - sink offset 0F8
		EventDispatcher<TESQuestStageEvent>					questStageEventSource;				// 0A50 - sink offset 100
		EventDispatcher<TESQuestStageItemDoneEvent>			questStageItemDoneEventSource;		// 0AA8 - sink offset 108
		EventDispatcher<TESQuestStartStopEvent>				questStartStopEventSource;			// 0B00 - sink offset 110
		EventDispatcher<TESResetEvent>						resetEventSource;					// 0B58 - sink offset 118
		EventDispatcher<TESResolveNPCTemplatesEvent>		resolveNPCTemplatesEventSource;		// 0BB0 - sink offset 120
		EventDispatcher<TESSceneEvent>						sceneEventSource;					// 0C08 - sink offset 128
		EventDispatcher<TESSceneActionEvent>				sceneActionEventSource;				// 0C60 - sink offset 130
		EventDispatcher<TESScenePhaseEvent>					scenePhaseEventSource;				// 0CB8 - sink offset 138
		EventDispatcher<TESSellEvent>						sellEventSource;					// 0D10 - sink offset 140
		EventDispatcher<TESSleepStartEvent>					sleepStartEventSource;				// 0D68 - sink offset 148
		EventDispatcher<TESSleepStopEvent>					sleepStopEventSource;				// 0DC0 - sink offset 150
		EventDispatcher<TESSpellCastEvent>					spellCastEventSource;				// 0E18 - sink offset 158
		EventDispatcher<TESPlayerBowShotEvent>				playerBowShotEventSource;			// 0E70 - sink offset 160
		EventDispatcher<TESTopicInfoEvent>					topicInfoEventSource;				// 0EC8 - sink offset 168
		EventDispatcher<TESTrackedStatsEvent>				trackedStatsEventSource;			// 0F20 - sink offset 170
		EventDispatcher<TESTrapHitEvent>					trapHitEventSource;					// 0F78 - sink offset 178
		EventDispatcher<TESTriggerEvent>					triggerEventSource;					// 0FD0 - sink offset 180
		EventDispatcher<TESTriggerEnterEvent>				triggerEnterEventSource;			// 1028 - sink offset 188
		EventDispatcher<TESTriggerLeaveEvent>				triggerLeaveEventSource;			// 1080 - sink offset 190
		EventDispatcher<TESUniqueIDChangeEvent>				uniqueIDChangeEventSource;			// 10D8 - sink offset 198
		EventDispatcher<TESWaitStartEvent>					waitStartEventSource;				// 1130 - sink offset 1A0
		EventDispatcher<TESWaitStopEvent>					waitStopEventSource;				// 1188 - sink offset 1A8
		EventDispatcher<TESSwitchRaceCompleteEvent>			switchRaceCompleteEventSource;		// 11E0 - sink offset 1B0
	};
#else
	extern EventDispatcher<TESEquipEvent>* g_equipEventDispatcher;
	extern EventDispatcher<TESUniqueIDChangeEvent>* g_uniqueIDChangedEventDispatcher;
#endif
}


#ifndef _WIN64
template <>
class BSTEventSink<RE::TESEquipEvent>
{
public:
	virtual ~BSTEventSink() {}
	virtual	EventResult ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher) = 0;
};


template <>
class BSTEventSink<RE::TESUniqueIDChangeEvent>
{
public:
	virtual ~BSTEventSink() {}
	virtual	EventResult ReceiveEvent(RE::TESUniqueIDChangeEvent* a_event, EventDispatcher<RE::TESUniqueIDChangeEvent>* a_dispatcher) = 0;
};
#endif
