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