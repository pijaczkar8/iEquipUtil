#include "RefHandleManager.h"

#include <exception>  // exception
#include <stdexcept>  // runtime_error, domain_error

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


	void RefHandleManager::Clear()
	{
		_formMap.clear();
		_activeHandles.reset();
		_lastHandle = kInvalidHandle;
	}


	ISerializableForm* RefHandleManager::LookupFormByHandle(UInt32 a_refHandle)
	{
		auto it = _formMap.find(a_refHandle);
		return it == _formMap.end() ? 0 : it->second;
	}


	UInt32 RefHandleManager::GetHandle(ISerializableForm* a_form)
	{
		UInt32 start = _lastHandle++;
		while ((_lastHandle == kInvalidHandle || _activeHandles[_lastHandle]) && _lastHandle != start) {
			++_lastHandle;
		}

		if (_lastHandle == start) {
			throw std::overflow_error("[FATAL ERROR] Ran out of reference handles!");
		} else {
			_formMap.insert({ _lastHandle, a_form });
			return _lastHandle;
		}
	}


	void RefHandleManager::ReturnHandle(UInt32 a_handle)
	{
		_activeHandles[a_handle] = false;
	}


	void RefHandleManager::SetHandleAsActive(UInt32 a_handle, ISerializableForm* a_form)
	{
		auto result = _formMap.insert({ a_handle, a_form });
		if (result.second) {
			_activeHandles[a_handle] = true;
		} else {
			throw std::runtime_error("Tried to claim handle already in use!");
		}
	}


	RefHandleManager::RefHandleManager() :
		_lastHandle(kInvalidHandle)
	{}


	RefHandleManager::~RefHandleManager()
	{}


	RefHandleManager* RefHandleManager::_singleton = 0;
}
