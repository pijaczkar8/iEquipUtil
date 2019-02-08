#include "RefHandleManager.h"

#include <exception>  // exception

#include "version.h"  // MAKE_STR


namespace iEquip
{
	RefHandleManager* RefHandleManager::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new RefHandleManager();
		}
		return _singleton;
	}


	void RefHandleManager::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	const char* RefHandleManager::ClassName() const
	{
		return MAKE_STR(RefHandleManager);
	}


	void RefHandleManager::Clear()
	{
		_activeHandles.reset();
		_nextHandle = 0;
	}


	bool RefHandleManager::Save(json& a_save)
	{
		try {
			a_save = {
				{ MAKE_STR(_activeHandles), _activeHandles.to_string() },
				{ MAKE_STR(_nextHandle), _nextHandle }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool RefHandleManager::Load(json& a_load)
	{
		try {
			if (!loadJsonObj<kMaxHandles>(a_load, MAKE_STR(_activeHandles), _activeHandles)) {
				return false;
			}

			if (!loadJsonObj(a_load, MAKE_STR(_nextHandle), _nextHandle)) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	ISerializableForm* RefHandleManager::LookupFormByHandle(UInt32 a_refHandle)
	{
		auto it = _formMap.find(a_refHandle);
		return it == _formMap.end() ? 0 : it->second;
	}


	void RefHandleManager::ReturnHandle(UInt32 a_handle)
	{
		_activeHandles[a_handle] = false;
	}


	RefHandleManager::RefHandleManager() :
		_nextHandle(0)
	{}


	RefHandleManager::~RefHandleManager()
	{}


	UInt32 RefHandleManager::GetHandle(ISerializableForm* a_form)
	{
		UInt32 start = _nextHandle;
		while (_activeHandles[_nextHandle] && _nextHandle != start) {
			++_nextHandle;
		}

		if (_nextHandle == start) {
			throw std::runtime_error("[FATAL ERROR] Ran out of reference handles!");
		} else {
			_formMap.insert({ _nextHandle, a_form });
			return _nextHandle++;
		}
	}


	RefHandleManager* RefHandleManager::_singleton = 0;
}
