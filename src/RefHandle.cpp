#include "RefHandle.h"

#include <exception>  // exception

#include "RefHandleManager.h"  // RefHandleManager
#include "version.h"  // MAKE_STR


namespace iEquip
{
	RefHandle::RefHandle(bool a_isActive, ISerializableForm* a_form) :
		_form(a_form),
		_handle(RefHandleManager::kInvalidHandle),
		_isActive(a_isActive)
	{}


	RefHandle::~RefHandle()
	{
		FreeHandle();
	}


	const char* RefHandle::ClassName() const
	{
		return MAKE_STR(RefHandle);
	}


	void RefHandle::Clear()
	{
		FreeHandle();
		_form = 0;
		_handle = 0;
		_isActive = false;
	}


	bool RefHandle::Save(json& a_save)
	{
		try {
			a_save = {
				{ MAKE_STR(_isActive), _isActive },
				{ MAKE_STR(_handle), _handle }
			};
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool RefHandle::Load(json& a_load)
	{
		try {
			if (!loadJsonObj(a_load, MAKE_STR(isActive), _isActive)) {
				return false;
			}

			if (!loadJsonObj(a_load, MAKE_STR(handle), _handle)) {
				return false;
			}

			if (_isActive) {
				RefHandleManager::GetSingleton()->SetHandleAsActive(_handle, _form);
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	UInt32 RefHandle::GetHandle() const
	{
		return _handle;
	}


	bool RefHandle::IsActive() const
	{
		return _isActive;
	}


	void RefHandle::AcquireHandle()
	{
		_isActive = true;
		_handle = RefHandleManager::GetSingleton()->GetHandle(_form);
	}


	void RefHandle::FreeHandle()
	{
		if (_isActive) {
			RefHandleManager* refManager = RefHandleManager::GetSingleton();
			refManager->ReturnHandle(_handle);
		}
	}
}
