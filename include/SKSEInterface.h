#pragma once

#include "gamethreads.h"  // TaskDelegate
#include "PluginAPI.h"  // SKSEInterface, PluginHandle, SKSEScaleformInterface, SKSEPapyrusInterface, SKSESerializationInterface, SKSETaskInterface, SKSEMessagingInterface, SKSEObjectInterface

#include <functional>  // function


class SKSE
{
public:
	using TaskFn = std::function<void()>;


	static bool							Set(const SKSEInterface* a_skse);
	static PluginHandle					GetPluginHandle();
	static SKSEScaleformInterface*		GetScaleformInterface();
	static SKSEPapyrusInterface*		GetPapyrusInterface();
	static SKSESerializationInterface*	GetSerializationInterface();
	static SKSETaskInterface*			GetTaskInterface();
	static SKSEMessagingInterface*		GetMessagingInterface();
	static SKSEObjectInterface*			GetObjectInterface();
	static void							AddTask(TaskFn a_fn);

private:
	class Task : public TaskDelegate
	{
	public:
		Task(TaskFn a_fn) :
			_fn(a_fn)
		{}


		virtual void Run() override
		{
			_fn();
		}


		virtual void Dispose() override
		{
			delete this;
		}

	private:
		TaskFn _fn;
	};


	inline static PluginHandle					_pluginHandle = kPluginHandle_Invalid;
	inline static SKSEScaleformInterface*		_scaleformInterface = 0;
	inline static SKSEPapyrusInterface*			_papyrusInterface = 0;
	inline static SKSESerializationInterface*	_serializationInterface = 0;
	inline static SKSETaskInterface*			_taskInterface = 0;
	inline static SKSEMessagingInterface*		_messagingInterface = 0;
	inline static SKSEObjectInterface*			_objectInterface = 0;
};
