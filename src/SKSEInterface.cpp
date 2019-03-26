#include "SKSEInterface.h"


bool SKSE::Set(const SKSEInterface* a_skse)
{
	_pluginHandle = a_skse->GetPluginHandle();

	_scaleformInterface = static_cast<SKSEScaleformInterface*>(a_skse->QueryInterface(kInterface_Scaleform));
	if (!_scaleformInterface) {
		_ERROR("[ERROR] Failed to query scaleform interface!\n");
		return false;
	}

	_papyrusInterface = static_cast<SKSEPapyrusInterface*>(a_skse->QueryInterface(kInterface_Papyrus));
	if (!_papyrusInterface) {
		_ERROR("[ERROR] Failed to query papyrus interface!\n");
		return false;
	}

	_serializationInterface = static_cast<SKSESerializationInterface*>(a_skse->QueryInterface(kInterface_Serialization));
	if (!_serializationInterface) {
		_ERROR("[ERROR] Failed to query serialization interface!\n");
		return false;
	}

	_taskInterface = static_cast<SKSETaskInterface*>(a_skse->QueryInterface(kInterface_Task));
	if (!_taskInterface) {
		_ERROR("[ERROR] Failed to query task interface!\n");
		return false;
	}

	_messagingInterface = static_cast<SKSEMessagingInterface*>(a_skse->QueryInterface(kInterface_Messaging));
	if (!_messagingInterface) {
		_ERROR("[ERROR] Failed to query messaging interface!\n");
		return false;
	}

	_objectInterface = static_cast<SKSEObjectInterface*>(a_skse->QueryInterface(kInterface_Object));
	if (!_objectInterface) {
		_ERROR("[ERROR] Failed to query object interface!\n");
		return false;
	}

	return true;
}


PluginHandle SKSE::GetPluginHandle()
{
	return _pluginHandle;
}


SKSEScaleformInterface* SKSE::GetScaleformInterface()
{
	return _scaleformInterface;
}


SKSEPapyrusInterface* SKSE::GetPapyrusInterface()
{
	return _papyrusInterface;
}


SKSESerializationInterface* SKSE::GetSerializationInterface()
{
	return _serializationInterface;
}


SKSETaskInterface* SKSE::GetTaskInterface()
{
	return _taskInterface;
}


SKSEMessagingInterface* SKSE::GetMessagingInterface()
{
	return _messagingInterface;
}


SKSEObjectInterface* SKSE::GetObjectInterface()
{
	return _objectInterface;
}


void SKSE::AddTask(TaskFn a_fn)
{
	_taskInterface->AddTask(new Task(a_fn));
}
