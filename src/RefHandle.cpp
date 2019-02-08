#include "RefHandle.h"

#include "RefHandleManager.h"  // RefHandleManager


namespace iEquip
{
	RefHandle::RefHandle(ISerializableForm* a_form) :
		_isActive(a_form != 0),
		_handle(0)
	{
		if (_isActive) {
			RefHandleManager* refManager = RefHandleManager::GetSingleton();
			_handle = refManager->GetHandle(a_form);
		}
	}


	RefHandle::~RefHandle()
	{
		if (_isActive) {
			RefHandleManager* refManager = RefHandleManager::GetSingleton();
			refManager->ReturnHandle(_handle);
		}
	}


	UInt32 RefHandle::GetHandle() const
	{
		return _handle;
	}
}
