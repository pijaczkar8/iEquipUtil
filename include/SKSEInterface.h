#pragma once

#include "PapyrusEvents.h"  // EventDispatcher, SKSEModCallbackEvent, SKSECameraEvent, SKSECrosshairRefEvent, SKSEActionEvent, SKSENiNodeUpdateEvent
#include "gamethreads.h"  // TaskDelegate
#include "PluginAPI.h"  // SKSEInterface, PluginHandle, SKSEScaleformInterface, SKSEPapyrusInterface, SKSESerializationInterface, SKSETaskInterface, SKSEMessagingInterface

#include <functional>  // function


class SKSE
{
public:
	using TaskFn = std::function<void()>;


	static bool										Init(const SKSEInterface* a_skse);

	static PluginHandle								GetPluginHandle();
	static UInt32									GetReleaseIndex();

	static SKSEScaleformInterface*					GetScaleformInterface();
	static SKSEPapyrusInterface*					GetPapyrusInterface();
	static SKSESerializationInterface*				GetSerializationInterface();

	static SKSETaskInterface*						GetTaskInterface();
	static void										AddTask(TaskFn a_fn);
	static void										AddTask(TaskDelegate* a_delegate);
	static void										AddTask(UIDelegate_v1* a_delegate);

	static SKSEMessagingInterface*					GetMessagingInterface();
	static EventDispatcher<SKSEModCallbackEvent>*	GetModCallbackEventSource();
	static EventDispatcher<SKSECameraEvent>*		GetCameraEventSource();
	static EventDispatcher<SKSECrosshairRefEvent>*	GetCrosshairRefEventSource();
	static EventDispatcher<SKSEActionEvent>*		GetActionEventSource();
	static EventDispatcher<SKSENiNodeUpdateEvent>*	GetNiNodeUpdateEventSource();

private:
	class Task : public TaskDelegate
	{
	public:
		Task(TaskFn a_fn);

		virtual void Run() override;
		virtual void Dispose() override;

	private:
		TaskFn _fn;
	};


	inline static PluginHandle								_pluginHandle = kPluginHandle_Invalid;
	inline static UInt32									_releaseIndex = 0;

	inline static SKSEScaleformInterface*					_scaleformInterface = 0;
	inline static SKSEPapyrusInterface*						_papyrusInterface = 0;
	inline static SKSESerializationInterface*				_serializationInterface = 0;
	inline static SKSETaskInterface*						_taskInterface = 0;

	inline static SKSEMessagingInterface*					_messagingInterface = 0;
	inline static EventDispatcher<SKSEModCallbackEvent>*	_modCallbackEventSource = 0;
	inline static EventDispatcher<SKSECameraEvent>*			_cameraEventSource = 0;
	inline static EventDispatcher<SKSECrosshairRefEvent>*	_crosshairRefEventSource = 0;
	inline static EventDispatcher<SKSEActionEvent>*			_actionEventSource = 0;
	inline static EventDispatcher<SKSENiNodeUpdateEvent>*	_niNodeUpdateEventSource = 0;
};
