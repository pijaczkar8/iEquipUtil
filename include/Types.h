#pragma once

#include <memory>  // shared_ptr


namespace Forms
{
	class ISerializableForm;

	typedef std::shared_ptr<ISerializableForm> SerializableFormPtr;
}
