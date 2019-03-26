#pragma once

#include "gamethreads.h"  // TaskDelegate
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // RegistrationSetHolder

#include "RefHandleManager.h"  // RefHandleManager::RefHandle

class TESForm;
class TESObjectWEAP;


class OnBoundWeaponEquippedRegSet : public RegistrationSetHolder<NullParameters>
{
public:
	static OnBoundWeaponEquippedRegSet* GetSingleton();

	void QueueEvent(UInt32 a_weaponType, UInt32 a_equipSlot);

private:
	using RegParams = NullParameters;


	OnBoundWeaponEquippedRegSet();
	OnBoundWeaponEquippedRegSet(const OnBoundWeaponEquippedRegSet&) = delete;
	OnBoundWeaponEquippedRegSet(OnBoundWeaponEquippedRegSet&&) = delete;

	OnBoundWeaponEquippedRegSet& operator=(const OnBoundWeaponEquippedRegSet&) = delete;
	OnBoundWeaponEquippedRegSet& operator=(OnBoundWeaponEquippedRegSet&&) = delete;


	BSFixedString _callback;
};


class OnBoundWeaponUnequippedRegSet : public RegistrationSetHolder<NullParameters>
{
public:
	static OnBoundWeaponUnequippedRegSet* GetSingleton();

	void QueueEvent(TESObjectWEAP* a_weap, UInt32 a_equipSlot);

private:
	using RegParams = NullParameters;


	OnBoundWeaponUnequippedRegSet();
	OnBoundWeaponUnequippedRegSet(const OnBoundWeaponUnequippedRegSet&) = delete;
	OnBoundWeaponUnequippedRegSet(OnBoundWeaponUnequippedRegSet&&) = delete;

	OnBoundWeaponUnequippedRegSet& operator=(const OnBoundWeaponUnequippedRegSet&) = delete;
	OnBoundWeaponUnequippedRegSet& operator=(OnBoundWeaponUnequippedRegSet&&) = delete;


	BSFixedString _callback;
};


class OnRefHandleActiveRegSet : public RegistrationSetHolder<NullParameters>
{
public:
	using RefHandle = RefHandleManager::RefHandle;


	static OnRefHandleActiveRegSet* GetSingleton();

	void QueueEvent(TESForm* a_baseItem, RefHandle a_refHandle, SInt32 a_itemCount);

private:
	using RegParams = NullParameters;


	OnRefHandleActiveRegSet();
	OnRefHandleActiveRegSet(const OnRefHandleActiveRegSet&) = delete;
	OnRefHandleActiveRegSet(OnRefHandleActiveRegSet&&) = delete;

	OnRefHandleActiveRegSet& operator=(const OnRefHandleActiveRegSet&) = delete;
	OnRefHandleActiveRegSet& operator=(OnRefHandleActiveRegSet&&) = delete;


	BSFixedString _callback;
};


class OnRefHandleInvalidatedRegSet : public RegistrationSetHolder<NullParameters>
{
public:
	using RefHandle = RefHandleManager::RefHandle;


	static OnRefHandleInvalidatedRegSet* GetSingleton();

	void QueueEvent(TESForm* a_baseItem, RefHandle a_refHandle);

private:
	using RegParams = NullParameters;


	OnRefHandleInvalidatedRegSet();
	OnRefHandleInvalidatedRegSet(const OnRefHandleInvalidatedRegSet&) = delete;
	OnRefHandleInvalidatedRegSet(OnRefHandleInvalidatedRegSet&&) = delete;

	OnRefHandleInvalidatedRegSet& operator=(const OnRefHandleInvalidatedRegSet&) = delete;
	OnRefHandleInvalidatedRegSet& operator=(OnRefHandleInvalidatedRegSet&&) = delete;


	BSFixedString _callback;
};
