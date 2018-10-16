# iEquip_SoulSeeker

Built with Skyrim 1.5.50, SKSE 2.0.8

Replace `BGSDefaultObjectManager::GetSingleton(void)` in `GameForms.cpp` with
```cpp
BGSDefaultObjectManager *BGSDefaultObjectManager::GetSingleton(void)
{
	// 81542B44FD6902A56B6B1464C37C41C529E9FD2A+31CB
	static RelocPtr<BGSDefaultObjectManager> g_BGSDefaultObjectManager(0x01EE4710);
	return g_BGSDefaultObjectManager;
}
```

Replace `TESAmmo` in `GameObjects.h` with
```cpp
// 128
class TESAmmo : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Ammo };

	// parents
	TESFullName					fullName;		// 030
	TESModelTextureSwap			texSwap;		// 040
	TESIcon						icon;			// 078
	BGSMessageIcon				messageIcon;	// 088
	TESValueForm				value;			// 0A0
	TESWeightForm				weight;			// 0B0
	BGSDestructibleObjectForm	destructible;	// 0C0
	BGSPickupPutdownSounds		sounds;			// 0D0
	TESDescription				description;	// 0E8
	BGSKeywordForm				keyword;		// 0F8

	// members

	// 10
	struct AmmoSettings
	{
		BGSProjectile	* projectile; // 0
		UInt32			flags;	// 8
		float			damage; // C
	};

	enum {
		kIgnoreNormalResist = (1 << 0),
		kNotPlayable		= (1 << 1),
		kNotBolt			= (1 << 2)
	};

	bool isBolt() { return (settings.flags & kNotBolt) != kNotBolt; }
	bool isPlayable() { return (settings.flags & kNotPlayable) != kNotPlayable; }

	AmmoSettings				settings;		// 110
	BSFixedString				unk120;			// 120
};
STATIC_ASSERT(offsetof(TESAmmo, fullName) == 0x30);
STATIC_ASSERT(offsetof(TESAmmo, texSwap) == 0x40);
STATIC_ASSERT(offsetof(TESAmmo, icon) == 0x78);
STATIC_ASSERT(offsetof(TESAmmo, messageIcon) == 0x88);
STATIC_ASSERT(offsetof(TESAmmo, value) == 0xA0);
STATIC_ASSERT(offsetof(TESAmmo, weight) == 0xB0);
STATIC_ASSERT(offsetof(TESAmmo, destructible) == 0xC0);
STATIC_ASSERT(offsetof(TESAmmo, sounds) == 0xD0);
STATIC_ASSERT(offsetof(TESAmmo, description) == 0XE8);
STATIC_ASSERT(offsetof(TESAmmo, keyword) == 0xF8);
STATIC_ASSERT(offsetof(TESAmmo, settings) == 0x110);
STATIC_ASSERT(offsetof(TESAmmo, unk120) == 0x120);
STATIC_ASSERT(sizeof(TESAmmo) == 0x128);
```